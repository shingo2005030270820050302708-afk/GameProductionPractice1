#include <DxLib.h>
#include "WoodBlock.h"
#include "../Block/BlockManager.h"
#include "../Map/MapParameter.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"

BlockData g_WoodBlock[BLOCK_MAX];
int g_WoodBlockHandle[B_BLOCK_TYPE_MAX] = { -1 };

BlockData* CreateWoodBlock(BlockType type, VECTOR pos)
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!g_WoodBlock[i].active)
        {
            g_WoodBlock[i].active = true;
            g_WoodBlock[i].blockType = type;
            g_WoodBlock[i].pos = pos;
            g_WoodBlock[i].vel = VGet(0, 0, 0);
            g_WoodBlock[i].state = BLOCK_STAY;
            g_WoodBlock[i].gravity = false;
            g_WoodBlock[i].handle = g_WoodBlockHandle[type];
            g_WoodBlock[i].breakable = (type == BREAKABLE_BLOCK);

            return &g_WoodBlock[i];
        }
    }

    // ‹ó‚«‚ª‚È‚©‚Á‚½
    return nullptr;
}
void InitWoodBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        g_WoodBlock[i].active = false;
        g_WoodBlock[i].pos = VGet(0, 0, 0);
        g_WoodBlock[i].vel = VGet(0, 0, 0);
        g_WoodBlock[i].state = BLOCK_STAY;
        g_WoodBlock[i].gravity = false;
        g_WoodBlock[i].breakable = false;
        g_WoodBlock[i].width = 32;
        g_WoodBlock[i].height = 32;
    }
}
void LoadWoodBlock()
{
    g_WoodBlockHandle[B_WOOD_BLOCK] = LoadGraph("Data/Block/WoodBlock.png");
    g_WoodBlockHandle[B_BREAKABLE_BLOCK] = LoadGraph("Data/Block/Breakable.png");

}


void StartWoodBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        g_WoodBlock[i].state = BLOCK_STAY;
        g_WoodBlock[i].gravity = false;
        g_WoodBlock[i].vel = VGet(0, 0, 0);
    }
    VECTOR pos = VGet(400, 700, 0);
    CreateWoodBlock(B_WOOD_BLOCK, pos);
}

void StepWoodBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        BlockData& b = g_WoodBlock[i];
        if (!b.active) continue;

        switch (b.state)
        {
        case BLOCK_LIFT:
            b.pos.x = GetPlayer()->posX + 15;
            b.pos.y = GetPlayer()->posY - 22;
            break;

        case BLOCK_THROW:
        case BLOCK_STAY:
        {
            if (b.gravity)
                b.vel.y += 0.5f;

            b.pos.x += b.vel.x;
            b.pos.y += b.vel.y;

            CheckBlockMapCollision(b);

            if (b.pos.y + b.height >= groundY)
            {
                b.pos.y = groundY - b.height;
                b.vel = VGet(0, 0, 0);
                b.gravity = false;
                b.state = BLOCK_STAY;
            }
        }
        break;
        }
    }
}

void UpdateWoodBlock(PlayerData& player)
{

    float liftPadding = 1.0f;

    for (int i = 0; i < BLOCK_MAX; i++)
    {
        BlockData& b = g_WoodBlock[i];
        if (!b.active) continue;

        // Ž‚¿ã‚°’†‚ÌƒuƒƒbƒN‚Í“–‚½‚è”»’è‚µ‚È‚¢
        if (b.state == BLOCK_LIFT) continue;

        float px = player.posX;
        float py = player.posY;
        float pw = player.boxCollision.width;
        float ph = player.boxCollision.height;

        float bx = b.pos.x;
        float by = b.pos.y;
        float bw = b.width;
        float bh = b.height;

        if (!CheckSquareSquare(px, py, pw, ph, bx, by, bw, bh))
            continue;

        // Õ“Ë‰ðŒˆ
        ResolvePlayerBlockCollision(&b);
    }
    PlayerData* p = GetPlayer();
    for (int bi = 0; bi < BLOCK_MAX; bi++)
    {
        BlockData& b = g_WoodBlock[bi];
        if (!b.active)
            continue;
        float px = player.posX + PLAYER_BOX_COLLISION_OFFSET_X - liftPadding;
        float py = player.posY + PLAYER_BOX_COLLISION_OFFSET_Y - liftPadding;
        float pw = player.boxCollision.width + liftPadding * 2;
        float ph = player.boxCollision.height + liftPadding * 2;
        // Block ‚Ì“–‚½‚è”»’è
        int bw, bh;
        GetGraphSize(b.handle, &bw, &bh);
        float bx = b.pos.x;
        float by = b.pos.y;

        bool hit =
            (px < bx + bw) &&
            (px + pw > bx) &&
            (py < by + bh) &&
            (py + ph > by);

        switch (b.state)
        {
        case BLOCK_STAY:
        {
            b.gravity = true;

            if (hit && IsTriggerKey(KEY_X))
            {
                b.state = BLOCK_LIFT;
                b.hold = true;
                b.gravity = false;
            }
        }
        break;

        case BLOCK_LIFT:
        {
            if (IsInputKey(KEY_DOWN) && IsTriggerKey(KEY_X))
            {

                b.state = BLOCK_STAY;
                b.hold = false;
            }
            else if (IsTriggerKey(KEY_X))
            {
                b.state = BLOCK_THROW;
                b.hold = false;
                b.gravity = true;

                b.vel.x = (p->isTurn ? -6.0f : 6.0f);
                b.vel.y = -8.0f;
            }
        }
        break;
        }
    }
}


void DrawWoodBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (g_WoodBlock[i].active && g_WoodBlock[i].handle != -1)
        {
            DrawGraph(
                (int)(g_WoodBlock[i].pos.x - camera.GetX()),
                (int)(g_WoodBlock[i].pos.y - camera.GetY()),
                g_WoodBlock[i].handle,
                TRUE
            );
        }
    }
}


void FinWoodBlock()
{
    for (int i = 0; i < BLOCK_TYPE_MAX; i++)
    {
        DeleteGraph(g_WoodBlockHandle[i]);
    }
}

