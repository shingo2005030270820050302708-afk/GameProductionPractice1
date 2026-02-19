#include <DxLib.h>
#include "Block.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"

BlockData g_Block[BLOCK_MAX];
int g_BlockHandle[BLOCK_TYPE_MAX] = { -1 };

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
}

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

    // ãÛÇ´Ç™Ç»Ç©Ç¡ÇΩ
    return nullptr;
}

void StepBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        BlockData& b = g_Block[i];
        if (!b.active)
            continue;

        switch (b.state)
        {
        case BLOCK_LIFT:
            // ÉvÉåÉCÉÑÅ[í«è]
            b.pos.x = GetPlayer()->posX;
            b.pos.y = GetPlayer()->posY - PLAYER_HEIGHT;
            break;

        case BLOCK_THROW:
            // èdóÕ
            if (b.gravity)
                b.vel.y += 0.5f;

            // à⁄ìÆ
            b.pos.x += b.vel.x;
            b.pos.y += b.vel.y;

            //// ìGÇ∆ÇÃìñÇΩÇËîªíË
            //for (int ei = 0; ei < ENEMY_MAX; ei++)
            //{
            //    EnemyData& e = g_Enemy[ei];
            //    if (!e.active)
            //        continue;

            //    if (CheckHitBlockEnemy(&b, &e))
            //    {
            //        e.hp -= 1;

            //        if (b.breakable)
            //            b.active = false;
            //        else
            //        {
            //            b.state = BLOCK_STAY;
            //            b.gravity = true;
            //        }

            //        break;
            //    }
            //}
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
            // íÕÇ›îªíËÇÕçLÇﬂÇ…Ç∑ÇÈ
            float px = player.posX + 10;
            float py = player.posY + 10;
            float pw = 30;
            float ph = 40;

            // Block ÇÃìñÇΩÇËîªíË
            int bw, bh;
            GetGraphSize(b.handle, &bw, &bh);
            float bx = b.pos.x;
            float by = b.pos.y;

            bool hit =
                (px < bx + bw) &&
                (px + pw > bx) &&
                (py < by + bh) &&
                (py + ph > by);

            if (hit && CheckHitKey(KEY_INPUT_X))
            {
                b.state = BLOCK_LIFT;
                b.gravity = false;
                b.vel = VGet(0, 0, 0);
                b.hold = true;   // Å© true Ç…Ç∑ÇÈ
            }
        }
        break;

        case BLOCK_LIFT:
            // â∫Å{X Å® íuÇ≠
            if (CheckHitKey(KEY_INPUT_DOWN) && CheckHitKey(KEY_INPUT_X))
            {
                b.gravity = false;
                b.hold = false;  // Å© false Ç…Ç∑ÇÈ
                b.state = BLOCK_STAY;
            }
            // X Å® ìäÇ∞ÇÈ
            else if (CheckHitKey(KEY_INPUT_X))
            {
                b.gravity = true;
                b.hold = false;  // Å© false Ç…Ç∑ÇÈ
                b.state = BLOCK_THROW;
            }
        break;
        case BLOCK_THROW:
            b.gravity = true;
            // ìäÇ∞ÇÈë¨ìx
            b.vel.x = (p->isTurn ? -6.0f : 6.0f);
            b.vel.y = -8.0f;
        }
    }
}


void DrawBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (g_Block[i].active && g_Block[i].handle != -1)
        {
            DrawGraph((int)g_Block[i].pos.x,
                (int)g_Block[i].pos.y, g_Block[i].handle, TRUE);
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