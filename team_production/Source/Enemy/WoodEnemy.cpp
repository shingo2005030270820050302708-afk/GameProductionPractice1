#include "DxLib.h"
#include "WoodEnemy.h"
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

WoodEnemyData g_WoodEnemyData[ENEMY_MAX];

void InitWoodEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_WoodEnemyData[i].active = false;
        g_WoodEnemyData[i].pos = VGet(0, 0, 0);
        g_WoodEnemyData[i].vel = VGet(0, 0, 0);
        g_WoodEnemyData[i].state = Idle;
        g_WoodEnemyData[i].gravity = false;
    }
}

void LoadWoodEnemy()
{
    int handle = LoadGraph("Data/Enemy/WoodEnemy.png");

    for (int i = 0; i < ENEMY_MAX; i++)
    {
        g_WoodEnemyData[i].handle = handle;
    }
}


void StartWoodEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        WoodEnemyData& e = g_WoodEnemyData[i];
        e.active = true;
        e.state = Idle;
        e.gravity = true;
        e.pos = VGet(200 + i * 100, 800, 0);
        e.vel = VGet(0.0f, 0.0f, 0.0f);

        e.boxCollision.width = 45.0f;
        e.boxCollision.height = 45.0f;
    }
}

void StepWoodEnemy(const PlayerData& player)
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        WoodEnemyData& e = g_WoodEnemyData[i];
        if (!e.active) continue;

        UpdateWoodEnemy(e, player);

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

void UpdateWoodEnemy(WoodEnemyData& e, const PlayerData& player)
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

void UpdateIdle(WoodEnemyData& e, const PlayerData& player)
{
    float dx = player.posX - e.pos.x;

    if (fabsf(dx) < 1200.0f)
    {
        e.state = Move;
    }

}

void UpdateMove(WoodEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;

    e.vel.x = (vx > 0) ? 0.5f : -0.5f;

    //ãﬂÇ√Ç¢ÇΩÇÁçUåÇÇ∑ÇÈ
    if (fabsf(vx) < 200.0f)
    {
        e.state = Attack;
    }

}

void UpdateAttack(WoodEnemyData& e, const PlayerData& player)
{
    float vx = player.posX - e.pos.x;

    e.vel.x = (vx > 0) ? 0.75f : -0.75f;

    // è≠Çµó£ÇÍÇΩÇÁí«ê’Ç…ñﬂÇÈ
    if (fabsf(vx) > 300.0f)
    {
        e.state = Move;
    }
}

void UpdateDead(WoodEnemyData& e, const PlayerData& player)
{
    e.deathTimer++;

    if (e.deathTimer > 30)
    {
        e.active = false;
    }
}

void DrawWoodEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (g_WoodEnemyData[i].active && g_WoodEnemyData[i].handle != -1)
        {
            DrawGraph(
                (int)(g_WoodEnemyData[i].pos.x - camera.GetX()),
                (int)(g_WoodEnemyData[i].pos.y - camera.GetY()),
                g_WoodEnemyData[i].handle,
                TRUE
            );
        }
    }
}

void FinWoodEnemy()
{
    if (ENEMY_MAX > 0 && g_WoodEnemyData[0].handle != -1)
    {
        DeleteGraph(g_WoodEnemyData[0].handle);
        for (int i = 0; i < ENEMY_MAX; i++)
        {
            g_WoodEnemyData[i].handle = -1;
        }
    }
}