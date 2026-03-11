#include <DxLib.h>
#include "FireFloor.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"
#include "../Block/Block.h"
#include "../Player/Player.h"
#include "../Block/BlockManager.h"
#include "../Block/BlockHub.h"

FireFloorData g_FireFloor[FIREFLOOR_MAX];
int g_FireFloorHandle = -1;
int g_FireFloorCoolHandle = -1; // 冷却後の画像ハンドル
//int fx = MAP_CHIP_WIDTH;   // 32
//int fy = MAP_CHIP_HEIGHT;  // 32

void InitFireFloor()
{
    for (int i = 0; i < FIREFLOOR_MAX; ++i)
    {
        g_FireFloor[i].active = false;
        g_FireFloor[i].handle = -1;
        g_FireFloor[i].pos = VGet(0, 0, 0);
        g_FireFloor[i].width = MAP_CHIP_WIDTH;
        g_FireFloor[i].height = MAP_CHIP_HEIGHT;
        g_FireFloor[i].damaging = true;
        g_FireFloor[i].coverBlockIndex = -1;
        g_FireFloor[i].damageTimer = 0;
        g_FireFloor[i].damageCooldown = 60;
    }
}

void LoadFireFloor()
{
    g_FireFloorHandle = LoadGraph("Data/Gimmick/FireFloor.png");
    //g_FireFloorCoolHandle = LoadGraph("Data/Gimmick/CoolFireFloor.png"); // 追加
}

FireFloorData* CreateFireFloor(VECTOR pos)
{
    for (int i = 0; i < FIREFLOOR_MAX; ++i)
    {
        if (!g_FireFloor[i].active)
        {
            g_FireFloor[i].active = true;
            g_FireFloor[i].pos = pos;
            g_FireFloor[i].handle = g_FireFloorHandle;
            g_FireFloor[i].damaging = true;
            g_FireFloor[i].coverBlockIndex = -1;
            g_FireFloor[i].damageTimer = 0;
            return &g_FireFloor[i];
        }
    }
    return nullptr;
}

void StartFireFloor()
{
    for (int i = 0; i < FIREFLOOR_MAX; i++)
        g_FireFloor[i].active = false;

    VECTOR pos = VGet(960, 832, 0);
    CreateFireFloor(pos);
}
void StepFireFloor()
{
    for (int i = 0; i < FIREFLOOR_MAX; ++i)
    {
        FireFloorData& f = g_FireFloor[i];
        if (!f.active) continue;

        if (f.damageTimer > 0) --f.damageTimer;

        // coverBlockIndex があれば、そのブロックがまだ存在しているか確認
        if (f.coverBlockIndex >= 0 && f.coverBlockIndex < BLOCK_MAX)
        {
            if (!g_Block[f.coverBlockIndex].active)
            {
                // ブロック消滅 -> 被覆解除（冷却状態は維持しない場合はここで復元する）
                f.coverBlockIndex = -1;
                // 被覆による無効化だけの場合はダメージ再開
                // (ただし Ice による冷却で非ダメージ化した場合は handle が切り替わっているので
                //  再開させたくないなら以下の行はコメントアウト)
                // f.damaging = true;
            }
            else
            {
                // 被覆中はダメージしない
                f.damaging = false;
            }
        }
    }
}

void UpdateFireFloor(PlayerData& player)
{
    // ブロックとの相互作用チェック
    for (int fi = 0; fi < FIREFLOOR_MAX; ++fi)
    {
        FireFloorData& f = g_FireFloor[fi];
        if (!f.active) continue;

        // 床矩形
        float fx = f.pos.x;
        float fy = f.pos.y;
        float fw = f.width;
        float fh = f.height;

        // ブロックとの当たり判定を走らせる
        for (int bi = 0; bi < BLOCK_MAX; ++bi)
        {
            BlockData& b = g_Block[bi];
            if (!b.active) continue;

            float bx = b.pos.x;
            float by = b.pos.y;
            float bw = b.width;
            float bh = b.height;

            if (!CheckSquareSquare(fx, fy, fw, fh, bx, by, bw, bh))
                continue;

            // FireFloor の矩形
            float fx2 = fx + fw;
            float fy2 = fy + fh;

            // Block の矩形
            float bx2 = bx + bw;
            float by2 = by + bh;

            bool overlapX = !(fx2 < bx || bx2 < fx);
            bool onTop = overlapX && fabs(by - fy) <= 1.0f;

            if (onTop)
            {
                f.handle = g_FireFloorCoolHandle;
                f.damaging = false;
            }
            // ▼ ブロックの種類ごとの処理
            switch (b.blockType)
            {
            case B_ICE_BLOCK:
                b.active = false;
                f.damaging = false;
                f.coverBlockIndex = -1;
                if (g_FireFloorCoolHandle != -1)
                    f.handle = g_FireFloorCoolHandle;
                break;

            case B_WOOD_BLOCK:
                b.active = false;
                break;

            case B_NORMAL_BLOCK:
                f.coverBlockIndex = bi;
                f.damaging = false;
                break;

            default:
                break;
            }
        }

        // プレイヤーとの当たり判定・ダメージ処理
        float px = player.posX + PLAYER_BOX_COLLISION_OFFSET_X;
        float py = player.posY + PLAYER_BOX_COLLISION_OFFSET_Y;
        float pw = player.boxCollision.width;
        float ph = player.boxCollision.height;

        bool hit =
            (px < fx + fw) &&
            (px + pw > fx) &&
            (py < fy + fh) &&
            (py + ph > fy);

        if (hit && f.damaging)
        {
            if (f.damageTimer == 0)
            {
                // ダメージ処理：HP 減少、ダメージ状態へ
                if (player.hp > 0) player.hp -= 1;
                player.state = DAMAGE;
                f.damageTimer = f.damageCooldown;
            }
        }
    }
}

void DrawFireFloor()
{
    extern Camera camera;
    for (int i = 0; i < FIREFLOOR_MAX; ++i)
    {
        FireFloorData& f = g_FireFloor[i];
        if (!f.active) continue;

        if (f.handle != -1)
        {
            f.pos = VGet(
                g_IceBlock->pos.x - camera.GetX(),
                f.pos.y,
                f.pos.z
            );
        }
    }
}

void FinFireFloor()
{
    if (g_FireFloorHandle != -1)
    {
        DeleteGraph(g_FireFloorHandle);
        g_FireFloorHandle = -1;
    }
    if (g_FireFloorCoolHandle != -1)
    {
        DeleteGraph(g_FireFloorCoolHandle);
        g_FireFloorCoolHandle = -1;
    }
}