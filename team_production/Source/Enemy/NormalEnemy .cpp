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
        g_NormalEnemyData[i].state = Move;
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

        e.active = false;
        e.state = Move;
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

        for (int j = 0; j < B_BLOCK_MAX; j++)
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
                // 投げられているブロック → 敵は死ぬ
                if (b.state == BLOCK_THROW)
                {
                    e.state = Dead;
                    return;
                }

                // 投げられていないブロック → 押し戻す
                ResolveEnemyBlockCollision(e, &b);


                break;
            }
        }

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
    case Move:
        UpdateMove(e, player);
        break;
    case Attack:
        UpdateAttack(e, player);
        break;
    case Dead:
        UpdateDead(e);
        return;
    }

}

void UpdateMove(NormalEnemyData& e, const PlayerData& player)
{
    e.walkTimer += 1.0f;

    if (e.walkTimer > e.changeInterval)
    {
        e.walkTimer = 0;

        int r = GetRand(2);

        if (r == 0) e.moveX = -0.5f;
        else if (r == 1) e.moveX = 0.5f;
        else e.moveX = 0.0f;

        e.isTurn = (e.moveX < 0);

        e.changeInterval = 60 + GetRand(120);
    }

    // 速度だけ設定
    const float speed = 0.5f;
    e.vel.x = e.moveX * speed;

    // 重力
    if (e.gravity)
    {
        e.vel.y += 0.5f;
        if (e.vel.y > 8.0f) e.vel.y = 8.0f;
    }

    // プレイヤーが近いなら Attack
    // ===== プレイヤーが近いなら Attack =====
    float px = player.posX + PLAYER_WIDTH * 0.5f;
    float py = player.posY + PLAYER_HEIGHT * 0.5f;

    float ex = e.pos.x + e.boxCollision.width * 0.5f;
    float ey = e.pos.y + e.boxCollision.height * 0.5f;

    float dx = px - ex;
    float dy = py - ey;
    float dist = sqrtf(dx * dx + dy * dy);

    const float attackRange = 80.0f;

    if (dist < attackRange)
    {
        e.state = Attack;
        e.vel.x = 0;
        return;
    }

}

void UpdateAttack(NormalEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;
    float dir = (vx > 0) ? 1.0f : -1.0f;

    e.isTurn = (dir < 0);

    if (IsBlockInFront(e, dir))
    {
        e.moveX = 0;
        e.vel.x = 0;
        return;
    }

    const float attackSpeed = 0.75f;
    e.moveX = dir;
    e.vel.x = e.moveX * attackSpeed;

    if (e.gravity)
    {
        e.vel.y += 0.5f;
        if (e.vel.y > 8.0f) e.vel.y = 8.0f;
    }

    float dx = fabsf(player.posX - e.pos.x);
    float dy = fabsf(player.posY - e.pos.y);

    if (dx > 300.0f || dy > 150.0f)
    {
        e.state = Move;
        e.moveX = 0;
    }

}


void UpdateDead(NormalEnemyData& e)
{
   e.active = false;

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