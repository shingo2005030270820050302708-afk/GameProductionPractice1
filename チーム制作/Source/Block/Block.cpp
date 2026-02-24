#include <DxLib.h>
#include "Block.h"
#include "BlockManager.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"

BlockData g_Block[BLOCK_MAX];
int g_BlockHandle[BLOCK_TYPE_MAX] = { -1 };

BlockData* CreateBlock(BlockType type, VECTOR pos)
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!g_Block[i].active)
        {
            g_Block[i].active = true;
            g_Block[i].blockType = type;
            g_Block[i].pos = pos;
            g_Block[i].vel = VGet(0, 0, 0);
            g_Block[i].state = BLOCK_STAY;
            g_Block[i].gravity = false;
            g_Block[i].handle = g_BlockHandle[type];
            g_Block[i].breakable = (type == BREAKABLE_BLOCK);

            return &g_Block[i];
        }
    }

    // ‹ó‚«‚ª‚È‚©‚Á‚½
    return nullptr;
}
void InitBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        g_Block[i].active = false;
        g_Block[i].pos = VGet(0, 0, 0);
        g_Block[i].vel = VGet(0, 0, 0);
        g_Block[i].state = BLOCK_STAY;
        g_Block[i].gravity = false;
        g_Block[i].breakable = false;
        g_Block[i].width = 32;
        g_Block[i].height = 32;
    }
}
void LoadBlock()
{
    g_BlockHandle[B_NORMAL_BLOCK] = LoadGraph("Data/Block/Normal.png");
    g_BlockHandle[BREAKABLE_BLOCK] = LoadGraph("Data/Block/Breakable.png");
}


void StartBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        g_Block[i].state = BLOCK_STAY;
        g_Block[i].gravity = false;
        g_Block[i].vel = VGet(0, 0, 0);
    }
    VECTOR pos = VGet(200, 800, 0);
    CreateBlock(B_NORMAL_BLOCK, pos);
}

void StepBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        BlockData& b = g_Block[i];
        if (!b.active) continue;

        switch (b.state)
        {
        case BLOCK_LIFT:
            b.pos.x = GetPlayer()->posX;
            b.pos.y = GetPlayer()->posY - PLAYER_HEIGHT;
            break;

        case BLOCK_THROW:
            if (b.gravity)
                b.vel.y += 0.5f;

            b.pos.x += b.vel.x;
            b.pos.y += b.vel.y;

            const float groundY = 850.0f;
            if (b.pos.y + b.height >= groundY)
            {
                b.pos.y = groundY - b.height;
                b.vel = VGet(0, 0, 0);
                b.gravity = false;
                b.state = BLOCK_STAY;
            }
            break;
        }
    }
}

void UpdateBlock(PlayerData& player)
{
    PlayerData* p = GetPlayer();
    for (int bi = 0; bi < BLOCK_MAX; bi++)
    {
        BlockData& b = g_Block[bi];
        if (!b.active)
            continue;

        switch (b.state)
        {
        case BLOCK_STAY:
        {
            b.gravity = true;
            // ’Í‚Ý”»’è‚ÍL‚ß‚É‚·‚é
            float px = player.posX + PLAYER_BOX_COLLISION_OFFSET_X;
            float py = player.posY + PLAYER_BOX_COLLISION_OFFSET_Y;
            float pw = player.boxCollision.width;
            float ph = player.boxCollision.height;

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

            if (hit && IsInputKey(KEY_X))
            {
                printfDx("‰Ÿ‚¹‚Ä‚é‚æ\n");
                b.state = BLOCK_LIFT;
                b.gravity = false;
                b.vel = VGet(0, 0, 0);
                b.hold = true;   // © true ‚É‚·‚é
            }
        }
        break;

        case BLOCK_LIFT:
            // ‰º{X ¨ ’u‚­
            if (IsTriggerKey(KEY_DOWN) && IsTriggerKey(KEY_X))
            {
                b.gravity = false;
                b.hold = false;  // © false ‚É‚·‚é
                b.state = BLOCK_STAY;
            }
            // X ¨ “Š‚°‚é
            else if (IsTriggerKey(KEY_X))
            {
                b.gravity = true;
                b.hold = false;  // © false ‚É‚·‚é
                b.state = BLOCK_THROW;

                b.vel.x = (p->isTurn ? -6.0f : 6.0f);
                b.vel.y = -8.0f;
            }
        break;

        case BLOCK_THROW:
        break;
        }
    }
}


void DrawBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (g_Block[i].active && g_Block[i].handle != -1)
        {
            DrawGraph(
                (int)(g_Block[i].pos.x - camera.GetX()),
                (int)(g_Block[i].pos.y - camera.GetY()),
                g_Block[i].handle,
                TRUE
            );
        }
    }
}


void FinBlock()
{
    for (int i = 0; i < BLOCK_TYPE_MAX; i++)
    {
        DeleteGraph(g_BlockHandle[i]);
    }
}

