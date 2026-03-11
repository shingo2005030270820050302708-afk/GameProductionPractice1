#include <DxLib.h>
#include "FireFloor.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"
#include "../Block/Block.h"
#include "../Player/Player.h"
#include "../Block/BlockManager.h"
#include "../Block/BlockHub.h"
#include "../Block/IceBlock.h"

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
        g_FireFloor[i].width = 96;
        g_FireFloor[i].height = 32;
        g_FireFloor[i].damaging = true;
        g_FireFloor[i].coverBlockIndex = -1;
        g_FireFloor[i].damageTimer = 0;
        g_FireFloor[i].damageCooldown = 60;
    }
}

void LoadFireFloor()
{
    g_FireFloorHandle = LoadGraph("Data/Gimmick/FireFloor.png");
    g_FireFloorCoolHandle = LoadGraph("Data/Gimmick/CoolFireFloor.png"); 
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

    VECTOR pos = VGet(32*8, 831, 0);
    CreateFireFloor(pos);
}
void StepFireFloor()
{
    for (int i = 0; i < FIREFLOOR_MAX; ++i)
    {
        FireFloorData& f = g_FireFloor[i];
        if (!f.active) continue;

        if (f.damageTimer > 0) --f.damageTimer;

        // デフォルトはダメージあり
        f.damaging = true;

        if (f.coverBlockIndex >= 0 && f.coverBlockIndex < BLOCK_MAX)
        {
            if (!g_Block[f.coverBlockIndex].active)
            {
                f.coverBlockIndex = -1;
            }
            else
            {
                f.damaging = false;
            }
        }
    }
}

void UpdateFireFloor(PlayerData& player)
{
    for (int fi = 0; fi < FIREFLOOR_MAX; ++fi)
    {
        FireFloorData& f = g_FireFloor[fi];
        if (!f.active) continue;

        float fx = f.pos.x;
        float fy = f.pos.y;
        float fw = f.width;
        float fh = f.height;

        // =========================
        // NormalBlock 判定
        // =========================
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

            bool onTop = fabs((by + bh) - fy) <= 2.0f;

            switch (b.blockType)
            {
            case B_NORMAL_BLOCK:
                if (onTop)
                {
                    f.coverBlockIndex = bi;
                    f.damaging = false;
                }
                break;

            case B_WOOD_BLOCK:
                b.active = false;
                break;

            default:
                break;
            }
        }

        // =========================
        // IceBlock 判定
        // =========================
        for (int bi = 0; bi < BLOCK_MAX; ++bi)
        {
            BlockData& b = g_IceBlock[bi];
            if (!b.active) continue;

            float bx = b.pos.x;
            float by = b.pos.y;
            float bw = b.width;
            float bh = b.height;

            if (!CheckSquareSquare(fx, fy, fw, fh, bx, by, bw, bh))
                continue;

            // IceBlock は消えて火を消す
            b.active = false;

            f.damaging = false;
            f.coverBlockIndex = -1;

            if (g_FireFloorCoolHandle != -1)
                f.handle = g_FireFloorCoolHandle;
        }

        // =========================
        // Player 判定
        // =========================
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
            player.state = DEAD;
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
            DrawGraph(
                (int)(f.pos.x - camera.GetX()),
                (int)(f.pos.y - camera.GetY()),
                f.handle,
                TRUE
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