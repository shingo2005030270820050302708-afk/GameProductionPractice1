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
        e.active = true;
        e.state = Idle;
        e.gravity = true;
        e.pos = VGet(200 + i * 100, 800, 0);
        e.vel = VGet(0.0f, 0.0f, 0.0f);

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
                e.state = Dead;
                e.active = false;
                break;
            }
        }

        for (int j = 0; j < B_BLOCK_MAX; j++)
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
        }

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
                e.state = Dead;
                e.active = false;
                break;
            }
        }

        for (int j = 0; j < B_BLOCK_MAX; j++)
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
        }

        for (int j = 0; j < B_BLOCK_MAX; j++)
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
        }
    }
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
    case Dead:
        UpdateDead(e, player);
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

    e.vel.x = (vx > 0) ? 0.5f : -0.5f;

    //ãﬂÇ√Ç¢ÇΩÇÁçUåÇÇ∑ÇÈ
    if (fabsf(vx) < 200.0f)
    {
        e.state = Attack;
    }

}

void UpdateAttack(NormalEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;

    e.vel.x = (vx > 0) ? 0.75f : -0.75f;

    // è≠Çµó£ÇÍÇΩÇÁí«ê’Ç…ñﬂÇÈ
    if (fabsf(vx) > 300.0f)
    {
        e.state = Move;
    }
}

void UpdateDead(NormalEnemyData& e, const PlayerData& player)
{
    e.deathTimer++;

    if (e.deathTimer > 30)
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