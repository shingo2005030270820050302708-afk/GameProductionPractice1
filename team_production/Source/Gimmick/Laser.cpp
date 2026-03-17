#include "Laser.h"
#include "../../Data/Camera/Camera.h"
#include "../Block/BlockManager.h"
#include "../Map/MapBlock.h"
#include "../Player/Player.h"

LaserData g_Laser[LASER_MAX];
int g_LaserHandle = -1;
int g_LaserMouthHandle = -1;
// 初期化
void InitLaser()
{
    for (int i = 0; i < LASER_MAX; ++i)
    {
        g_Laser[i].active = false;
        g_Laser[i].handle = -1;
        g_Laser[i].width = 10.0f;
        g_Laser[i].damaging = true;
        g_Laser[i].coverBlockIndex = -1;
    }
}

// 画像読み込み
void LoadLaser()
{
    g_LaserHandle = LoadGraph("Data/Gimmick/Laser.png");
    g_LaserMouthHandle = LoadGraph("Data/Gimmick/LaserMouth.png");
}

// 作成
LaserData* CreateLaser(VECTOR start, float length)
{
    for (int i = 0; i < LASER_MAX; ++i)
    {
        if (!g_Laser[i].active)
        {
            g_Laser[i].active = true;
            g_Laser[i].start = start;
            g_Laser[i].end = VGet(start.x, start.y + length, 0);
            g_Laser[i].width = 10.0f;
            g_Laser[i].damaging = true;
            g_Laser[i].coverBlockIndex = -1;
            g_Laser[i].handle = g_LaserHandle;
            return &g_Laser[i];
        }
    }
    return nullptr;
}

// 二重登録防止
LaserData* CreateLaserIfNotExist(VECTOR start, float length)
{
    for (int i = 0; i < LASER_MAX; ++i)
    {
        if (g_Laser[i].active &&
            g_Laser[i].start.x == start.x &&
            g_Laser[i].start.y == start.y)
        {
            return &g_Laser[i];
        }
    }
    return CreateLaser(start, length);
}

void UpdateLaser(PlayerData& player)
{
    for (int i = 0; i < LASER_MAX; ++i)
    {
        LaserData& l = g_Laser[i];
        if (!l.active) continue;

        float laserLeft = l.start.x - l.width / 2.0f;
        float laserRight = l.start.x + l.width / 2.0f;

        float maxLength = 5000.0f;
        l.end.y = l.start.y + maxLength;
        l.coverBlockIndex = -1;

        // まず動かせるブロックでレーザー長さを制御
        for (int bi = 0; bi < BLOCK_MAX; ++bi)
        {
            BlockData& b = g_Block[bi];
            if (!b.active) continue;

            if (laserRight > b.pos.x && laserLeft < b.pos.x + b.width)
            {
                if (b.pos.y < l.end.y && b.pos.y > l.start.y)
                {
                    l.end.y = b.pos.y; // ブロックで途切れる

                    if (player.holdingBlock == &b)
                    {
                        l.coverBlockIndex = bi; // プレイヤー防御用
                    }
                }
            }
        }

        // 次にマップチップでもレーザー長さを制御
        for (int mi = 0; mi < BLOCK_MAX; ++mi)
        {
            BlockData& mb = g_MapBlocks[mi];
            if (!mb.active) continue;

            if (laserRight > mb.pos.x && laserLeft < mb.pos.x + mb.width)
            {
                if (mb.pos.y < l.end.y && mb.pos.y > l.start.y)
                {
                    l.end.y = mb.pos.y; // マップブロックで途切れる
                }
            }
        }

        // プレイヤー判定
        float px = player.posX + PLAYER_BOX_COLLISION_OFFSET_X;
        float py = player.posY + PLAYER_BOX_COLLISION_OFFSET_Y;
        float pw = player.boxCollision.width;
        float ph = player.boxCollision.height;

        bool playerHit = (px + pw > laserLeft && px < laserRight &&
            py + ph > l.start.y && py < l.end.y);

        // 持っているブロックでプレイヤーを防御
        if (player.holdingBlock != nullptr)
        {
            float bx = player.holdingBlock->pos.x;
            float by = player.holdingBlock->pos.y;
            float bw = player.holdingBlock->width;
            float bh = player.holdingBlock->height;

            if (!(px + pw < bx || px > bx + bw || py + ph < by || py > by + bh))
            {
                playerHit = false; // ダメージ判定のみ無効
            }
        }

        if (playerHit && player.state == NORMAL)
        {
            player.state = DEAD;
            player.deadTimer = 0;
            player.moveX = 0;
            player.moveY = -6.0f;
        }
    }
}

// 描画は常に l.end.y まで描画する
void DrawLaser()
{
    extern Camera camera;

    for (int i = 0; i < LASER_MAX; ++i)
    {
        LaserData& l = g_Laser[i];
        if (!l.active) continue;
        if (l.handle == -1) continue;

        // レーザー本体
        DrawBox(
            (int)(l.start.x - l.width / 2 - camera.GetX()),
            (int)(l.start.y - camera.GetY()),
            (int)(l.start.x + l.width / 2 - camera.GetX()),
            (int)(l.end.y - camera.GetY()),
            GetColor(255, 0, 0),
            TRUE
        );

        // 発射口描画
        if (g_LaserMouthHandle != -1)
        {
            int mouthW, mouthH;
            GetGraphSize(g_LaserMouthHandle, &mouthW, &mouthH);
            DrawGraph(
                (int)(l.start.x - mouthW / 2 - camera.GetX()),
                (int)(l.start.y - mouthH / 2 - camera.GetY()),
                g_LaserMouthHandle,
                TRUE
            );
        }
    }
}
// 空関数（FireFloor互換）
void StartLaser() {}
void StepLaser() {}

// 終了処理
void FinLaser()
{
    if (g_LaserHandle != -1)
    {
        DeleteGraph(g_LaserHandle);
        g_LaserHandle = -1;
    }
    if (g_LaserMouthHandle != -1)
    {
        DeleteGraph(g_LaserMouthHandle);
        g_LaserMouthHandle = -1;
    }
}