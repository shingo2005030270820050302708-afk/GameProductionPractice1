#include "DxLib.h"
#include "FireEnemy.h"
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

FireEnemyData g_FireEnemyData[ENEMY_MAX];

void InitFireEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_FireEnemyData[i].active = false;
        g_FireEnemyData[i].pos = VGet(0, 0, 0);
        g_FireEnemyData[i].vel = VGet(0, 0, 0);
        g_FireEnemyData[i].state = Idle;
        g_FireEnemyData[i].gravity = false;
    }
}

void LoadFireEnemy()
{
    int handle = LoadGraph("Data/Enemy/FireEnemy.png");

    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_FireEnemyData[i].handle = handle;
    }
}


void StartFireEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        FireEnemyData& e = g_FireEnemyData[i];
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
        FireEnemyData& e = g_FireEnemyData[i];
        if (!e.active) continue;

        UpdateFireEnemy(e, player);

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
    }
}

void UpdateFireEnemy(FireEnemyData& e, const PlayerData& player)
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

void UpdateIdle(FireEnemyData& e, const PlayerData& player)
{
    float dx = player.posX - e.pos.x;

    if (fabsf(dx) < 1200.0f)
    {
        e.state = Move;
    }

}

void UpdateMove(FireEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;

    e.vel.x = (vx > 0) ? 0.5f : -0.5f;

    //ãﬂÇ√Ç¢ÇΩÇÁçUåÇÇ∑ÇÈ
    if (fabsf(vx) < 200.0f)
    {
        e.state = Attack;
    }

}

void UpdateAttack(FireEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;

    e.vel.x = (vx > 0) ? 0.75f : -0.75f;

    // è≠Çµó£ÇÍÇΩÇÁí«ê’Ç…ñﬂÇÈ
    if (fabsf(vx) > 300.0f)
    {
        e.state = Move;
    }
}

void UpdateDead(FireEnemyData& e, const PlayerData& player)
{
    e.deathTimer++;

    if (e.deathTimer > 30)
    {
        e.active = false;
    }
}

void DrawFireEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_FireEnemyData[i].active && g_FireEnemyData[i].handle != -1)
        {
            DrawGraph(
                (int)(g_FireEnemyData[i].pos.x - camera.GetX()),
                (int)(g_FireEnemyData[i].pos.y - camera.GetY()),
                g_FireEnemyData[i].handle,
                TRUE
            );
        }
    }
}

void FinFireEnemy()
{
    if (ENEMY_MAX > 0 && g_FireEnemyData[0].handle != -1)
    {
        DeleteGraph(g_FireEnemyData[0].handle);
        for (int i = 0; i < ENEMY_MAX; i++)
        {
            g_FireEnemyData[i].handle = -1;
        }
    }
}