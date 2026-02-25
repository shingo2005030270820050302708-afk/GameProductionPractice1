#include <DxLib.h>
#include "Block.h"
#include "BlockManager.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"

BlockData g_Block[BLOCK_MAX];
int g_BlockHandle[BLOCK_TYPE_MAX] = { -1 };

float groundY = 832.0f;

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
        case BLOCK_PUSH:
            b.pos.x = GetPlayer()->posX + 65;
            break;
        case BLOCK_LIFT:
            b.pos.x = GetPlayer()->posX + 15;
            b.pos.y = GetPlayer()->posY - 25;
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
void CheckBlockMapCollision(BlockData& b)
{
    if (!b.active) return;

    float bx = b.pos.x;
    float by = b.pos.y;
    float bw = b.width;
    float bh = b.height;

    int leftTile = (int)(bx / MAP_CHIP_WIDTH);
    int rightTile = (int)((bx + bw - 1) / MAP_CHIP_WIDTH);
    int topTile = (int)(by / MAP_CHIP_HEIGHT);
    int bottomTile = (int)((by + bh - 1) / MAP_CHIP_HEIGHT);

    for (int y = topTile; y <= bottomTile; y++)
    {
        for (int x = leftTile; x <= rightTile; x++)
        {
            MapChipData chip = GetMapChipData(x, y);
            if (chip.mapChip == MAP_CHIP_NONE) continue;
            if (!chip.data || !chip.data->active) continue;

            BlockData* mapBlock = chip.data;
            if (CheckSquareSquare(bx, by, bw, bh, mapBlock->pos.x, mapBlock->pos.y, mapBlock->width, mapBlock->height))
            {
                float overlapTop = (by + bh) - mapBlock->pos.y;
                float overlapBottom = (mapBlock->pos.y + mapBlock->height) - by;
                float overlapLeft = (bx + bw) - mapBlock->pos.x;
                float overlapRight = (mapBlock->pos.x + mapBlock->width) - bx;

                float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
                float minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

                if (minOverlapY < minOverlapX)
                {
                    if (overlapTop < overlapBottom)
                    {
                        b.pos.y = mapBlock->pos.y - bh;
                        b.vel.y = 0;
                        b.gravity = false;
                        b.state = BLOCK_STAY;
                    }
                    else
                    {
                        b.pos.y = mapBlock->pos.y + mapBlock->height;
                        b.vel.y = 0;
                    }
                }
                else
                {
                    if (overlapLeft < overlapRight)
                        b.pos.x = mapBlock->pos.x - bw;
                    else
                        b.pos.x = mapBlock->pos.x + mapBlock->width;

                    b.vel.x = 0;
                }
            }
        }
    }
}

void UpdateBlock(PlayerData& player)
{

    float liftPadding = 1.0f;

    for (int i = 0; i < BLOCK_MAX; i++)
    {
        BlockData& b = g_Block[i];
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
        BlockData& b = g_Block[bi];
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

            if (hit && IsTriggerKey(KEY_C))
            {
                b.state = BLOCK_PUSH;
                b.hold = true;
                b.gravity = false;
            }

            if (hit && IsTriggerKey(KEY_X))
            {
                b.state = BLOCK_LIFT;
                b.hold = true;
                b.gravity = false;
            }
        }
        break;

        case BLOCK_PUSH:
        {
            if (IsInputKey(KEY_DOWN) && IsTriggerKey(KEY_X))
            {

                b.state = BLOCK_STAY;
                b.hold = false;
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

