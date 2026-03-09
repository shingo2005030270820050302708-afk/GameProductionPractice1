#include "DxLib.h"
#include "IceEnemy.h"
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

IceEnemyData g_IceEnemyData[ENEMY_MAX];

void InitIceEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_IceEnemyData[i].active = false;
        g_IceEnemyData[i].pos = VGet(0, 0, 0);
        g_IceEnemyData[i].vel = VGet(0, 0, 0);
        g_IceEnemyData[i].state = Idle;
        g_IceEnemyData[i].gravity = false;
    }
}

void LoadIceEnemy()
{
    int handle = LoadGraph("Data/Enemy/IceEnemy.png");

    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_IceEnemyData[i].handle = handle;
    }
}


void StartIceEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        IceEnemyData& e = g_IceEnemyData[i];
        e.active = true;
        e.state = Idle;
        e.gravity = true;
        e.pos = VGet(200 + i * 100, 800, 0);
        e.vel = VGet(0.0f, 0.0f, 0.0f);

        e.boxCollision.width = 45.0f;
        e.boxCollision.height = 45.0f;
    }
}

void StepIceEnemy(const PlayerData& player)
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        IceEnemyData& e = g_IceEnemyData[i];
        if (!e.active) continue;

        UpdateIceEnemy(e, player);

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
    }
}

void UpdateIceEnemy(IceEnemyData& e, const PlayerData& player)
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
        UpdateDead(e, player);
        break;
    }

}

void UpdateIdle(IceEnemyData& e, const PlayerData& player)
{
    float dx = player.posX - e.pos.x;

    if (fabsf(dx) < 1200.0f)
    {
        e.state = Move;
    }

}

void UpdateMove(IceEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;

    e.vel.x = (vx > 0) ? 0.5f : -0.5f;

    //ãﬂÇ√Ç¢ÇΩÇÁçUåÇÇ∑ÇÈ
    if (fabsf(vx) < 200.0f)
    {
        e.state = Attack;
    }

}

void UpdateAttack(IceEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;

    e.vel.x = (vx > 0) ? 0.75f : -0.75f;

    // è≠Çµó£ÇÍÇΩÇÁí«ê’Ç…ñﬂÇÈ
    if (fabsf(vx) > 300.0f)
    {
        e.state = Move;
    }
}

void UpdateDead(IceEnemyData& e, const PlayerData& player)
{
    e.deathTimer++;

    if (e.deathTimer > 30)
    {
        e.active = false;
    }
}

void CheckEnemyMapCollision(IceEnemyData& e)
{
}

void DrawIceEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_IceEnemyData[i].active && g_IceEnemyData[i].handle != -1)
        {
            DrawGraph(
                (int)(g_IceEnemyData[i].pos.x - camera.GetX()),
                (int)(g_IceEnemyData[i].pos.y - camera.GetY()),
                g_IceEnemyData[i].handle,
                TRUE
            );
        }
    }
}

void FinIceEnemy()
{
    if (ENEMY_MAX > 0 && g_IceEnemyData[0].handle != -1)
    {
        DeleteGraph(g_IceEnemyData[0].handle);
        for (int i = 0; i < ENEMY_MAX; i++)
        {
            g_IceEnemyData[i].handle = -1;
        }
    }
}