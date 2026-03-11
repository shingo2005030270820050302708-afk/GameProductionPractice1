#include "DxLib.h"
#include "NormalEnemy.h"
#include "../Player/Player.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"
#include "../Map/MapChip.h"
#include "../Block/Block.h"
#include "../Block/IceBlock.h"
#include "../Block/IronBlock.h"
#include "../Block/WoodBlock.h"
#include "../Block/BlockHub.h"
#include "../Block/FireBlock.h"
#include "../Block/BlockManager.h"

NormalEnemyData g_NormalEnemyData[ENEMY_MAX];

void InitNormalEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_NormalEnemyData[i].active = false;
        g_NormalEnemyData[i].pos = VGet(0, 0, 0);
        g_NormalEnemyData[i].vel = VGet(0, 0, 0);
        g_NormalEnemyData[i].state = Idle;
        g_NormalEnemyData[i].gravity = false;
    }
}

void LoadNormalEnemy()
{
    int handle = LoadGraph("Data/Enemy/Enemy.png");

    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_NormalEnemyData[i].handle = handle;
    }
}


void StartNormalEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        NormalEnemyData& e = g_NormalEnemyData[i];

        e.active = false;        // ← ここ重要！最初は全員 inactive
        e.state = Idle;
        e.gravity = true;
        e.vel = VGet(0.0f, 0.0f, 0.0f);
        e.deathTimer = 0;

        e.boxCollision.width = 45.0f;
        e.boxCollision.height = 45.0f;
    }
}

void StepNormalEnemy(const PlayerData& player)
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        NormalEnemyData& e = g_NormalEnemyData[i];
        if (!e.active) continue;

        UpdateNormalEnemy(e, player);

        if (e.gravity)
            e.vel.y += 1.0f;

        e.pos = VAdd(e.pos, e.vel);

        CheckEnemyMapCollision(e);

        float ex = e.pos.x;
        float ey = e.pos.y;             
        float ew = e.boxCollision.width;
        float eh = e.boxCollision.height;


        for (int j = 0; j < B_BLOCK_MAX; j++)
        {
            BlockData& b = g_Block[j];
            if (!b.active) continue;
            if (b.state != BLOCK_THROW) continue;

            float bx = b.pos.x;
            float by = b.pos.y;
            float bw = b.width;
            float bh = b.height;

            if (CheckSquareSquare(ex, ey, ew, eh, bx, by, bw, bh))
            {
                if (b.state == BLOCK_THROW)
                {
                    // 投げられているブロック → 敵は死ぬ
                    e.state = Dead;
                }
                else
                {
                    // 投げられていないブロック → 地形扱いで押し戻す
                    ResolveEnemyBlockCollision(e, &b);
                }

                break;
            }
        }

        /*for (int j = 0; j < B_BLOCK_MAX; j++)
        {
            BlockData& b = g_FireBlock[j];
            if (!b.active) continue;
            if (b.state != BLOCK_THROW) continue;

            float bx = b.pos.x;
            float by = b.pos.y;
            float bw = b.width;
            float bh = b.height;

            if (CheckSquareSquare(ex, ey, ew, eh, bx, by, bw, bh))
            {
                e.state = Dead;
                e.active = false;
                break;
            }
        }*/

        /*for (int j = 0; j < B_BLOCK_MAX; j++)
        {
            BlockData& b = g_IceBlock[j];
            if (!b.active) continue;
            if (b.state != BLOCK_THROW) continue;

            float bx = b.pos.x;
            float by = b.pos.y;
            float bw = b.width;
            float bh = b.height;

            if (CheckSquareSquare(ex, ey, ew, eh, bx, by, bw, bh))
            {
                e.state = Dead;
                e.active = false;
                break;
            }
        }*/

        /*for (int j = 0; j < B_BLOCK_MAX; j++)
        {
            BlockData& b = g_IronBlock[j];
            if (!b.active) continue;
            if (b.state != BLOCK_THROW) continue;

            float bx = b.pos.x;
            float by = b.pos.y;
            float bw = b.width;
            float bh = b.height;

            if (CheckSquareSquare(ex, ey, ew, eh, bx, by, bw, bh))
            {
                e.state = Dead;
                e.active = false;
                break;
            }
        }*/

        /*for (int j = 0; j < B_BLOCK_MAX; j++)
        {
            BlockData& b = g_WoodBlock[j];
            if (!b.active) continue;
            if (b.state != BLOCK_THROW) continue;

            float bx = b.pos.x;
            float by = b.pos.y;
            float bw = b.width;
            float bh = b.height;

            if (CheckSquareSquare(ex, ey, ew, eh, bx, by, bw, bh))
            {
                e.state = Dead;
                e.active = false;
                break;
            }
        }*/
    }
}

bool IsBlockInFront(const NormalEnemyData& e, float dirX)
{
    float ex = e.pos.x;
    float ey = e.pos.y;
    float ew = e.boxCollision.width;
    float eh = e.boxCollision.height;

    // 敵の前方に小さな判定を置く
    float checkX = ex + (dirX > 0 ? ew + 2 : -2);
    float checkY = ey + eh * 0.5f;

    // タイル座標に変換
    int tileX = (int)(checkX / MAP_CHIP_WIDTH);
    int tileY = (int)(checkY / MAP_CHIP_HEIGHT);

    MapChipData chip = GetMapChipData(tileX, tileY);
    if (chip.mapChip == MAP_CHIP_NONE) return false;

    BlockData* block = chip.data;
    if (!block || !block->active) return false;

    return true;
}

void UpdateNormalEnemy(NormalEnemyData& e, const PlayerData& player)
{
    switch (e.state)
    {
    case Idle:
        UpdateIdle(e, player);
        break;
    case Move:
        UpdateMove(e, player);
        break;
    case Attack:
        UpdateAttack(e, player);
        break;
    case Dead:
        UpdateDead(e);
        break;
    }

}

void UpdateIdle(NormalEnemyData& e, const PlayerData& player)
{
    float dx = player.posX - e.pos.x;

    if (fabsf(dx) < 1200.0f)
    {
        e.state = Move;
    }

}

void UpdateMove(NormalEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;
    float dir = (vx > 0) ? 1.0f : -1.0f;

    // 目の前にブロックがあるなら止まる
    if (IsBlockInFront(e, dir))
    {
        e.vel.x = 0;
        return;
    }

    // 何もなければ歩く
    e.vel.x = dir * 0.5f;

    if (fabsf(vx) < 500.0f)
    {
        e.state = Attack;
    }
}

void UpdateAttack(NormalEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;
    float dir = (vx > 0) ? 1.0f : -1.0f;

    // 目の前にブロックがあるなら止まる
    if (IsBlockInFront(e, dir))
    {
        e.vel.x = 0;
        return;
    }

    e.vel.x = dir * 0.75f;

    if (fabsf(vx) > 500.0f)
    {
        e.state = Move;
    }
}


void UpdateDead(NormalEnemyData& e)
{
    e.deathTimer++;

    if (e.deathTimer > 15)
    {
        e.active = false;
    }
}

void DrawNormalEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_NormalEnemyData[i].active && g_NormalEnemyData[i].handle != -1)
        {
            DrawGraph(
                (int)(g_NormalEnemyData[i].pos.x - camera.GetX()),
                (int)(g_NormalEnemyData[i].pos.y - camera.GetY()),
                g_NormalEnemyData[i].handle,
                TRUE
            );
        }
    }
}

void FinNormalEnemy()
{
    if (ENEMY_MAX > 0 && g_NormalEnemyData[0].handle != -1)
    {
        DeleteGraph(g_NormalEnemyData[0].handle);
        for (int i = 0; i < ENEMY_MAX; i++)
        {
            g_NormalEnemyData[i].handle = -1;
        }
    }
}