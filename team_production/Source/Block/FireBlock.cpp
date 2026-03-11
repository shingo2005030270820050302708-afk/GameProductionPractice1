#include <DxLib.h>
#include "FireBlock.h"
#include "../Block/BlockManager.h"
#include "../Map/MapParameter.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"


BlockData g_FireBlock[BLOCK_MAX];
int g_FireBlockHandle[B_BLOCK_TYPE_MAX] = { -1 };

BlockData* CreateFireBlock(BlockType type, VECTOR pos)
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!g_FireBlock[i].active)
        {
            g_FireBlock[i].active = true;
            g_FireBlock[i].blockType = type;
            g_FireBlock[i].pos = pos;
            g_FireBlock[i].vel = VGet(0, 0, 0);
            g_FireBlock[i].state = BLOCK_STAY;
            g_FireBlock[i].gravity = false;
            g_FireBlock[i].handle = g_FireBlockHandle[type];
            g_FireBlock[i].breakable = (type == BREAKABLE_BLOCK);

            return &g_FireBlock[i];
        }
    }

    // 空きがなかった
    return nullptr;
}
void InitFireBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        g_FireBlock[i].active = false;
        g_FireBlock[i].pos = VGet(0, 0, 0);
        g_FireBlock[i].vel = VGet(0, 0, 0);
        g_FireBlock[i].state = BLOCK_STAY;
        g_FireBlock[i].gravity = false;
        g_FireBlock[i].breakable = false;
        g_FireBlock[i].width = 32;
        g_FireBlock[i].height = 32;
    }
}
void LoadFireBlock()
{
    g_FireBlockHandle[B_FIRE_BLOCK] = LoadGraph("Data/Block/FireBlock.png");
}


void StartFireBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        g_FireBlock[i].state = BLOCK_STAY;
        g_FireBlock[i].gravity = false;
        g_FireBlock[i].vel = VGet(0, 0, 0);
    }
    VECTOR pos = VGet(300, 300, 0);
    CreateFireBlock(B_FIRE_BLOCK, pos);
}

void StepFireBlock()
{
   /* for (int i = 0; i < BLOCK_MAX; i++)
    {
        BlockData& b = g_FireBlock[i];
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
    }*/
}


void UpdateFireBlock(PlayerData& player)
{

    float liftPadding = 35.0f;

    for (int i = 0; i < BLOCK_MAX; i++)
    {
        BlockData& b = g_FireBlock[i];
        if (!b.active) continue;

        // 持ち上げ中のブロックは当たり判定しない
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

        // 衝突解決
       
    }
    PlayerData* p = GetPlayer();
    for (int bi = 0; bi < BLOCK_MAX; bi++)
    {
        BlockData& b = g_FireBlock[bi];
        if (!b.active)
            continue;
        float px = player.posX + PLAYER_BOX_COLLISION_OFFSET_X - liftPadding;
        float py = player.posY + PLAYER_BOX_COLLISION_OFFSET_Y - liftPadding;
        float pw = player.boxCollision.width + liftPadding * 2;
        float ph = player.boxCollision.height + liftPadding * 2;
        // Block の当たり判定
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

            if (player.holdingBlock == nullptr && hit && IsTriggerKey(KEY_X))
            {
                b.state = BLOCK_LIFT;
                b.hold = true;
                b.gravity = false;

                player.holdingBlock = &b;
                return;   
            }
        }
        break;

        case BLOCK_LIFT:
        {
            if (IsInputKey(KEY_DOWN) && IsTriggerKey(KEY_X))
            {
                b.state = BLOCK_STAY;
                b.hold = false;
                player.holdingBlock = nullptr;

                //プレイヤーと重ならないように位置補正
                float px = player.posX;
                float py = player.posY;
                float pw = player.boxCollision.width;
                float ph = player.boxCollision.height;

                float bx = b.pos.x;
                float by = b.pos.y;
                float bw = b.width;
                float bh = b.height;

                // 下に置く
                b.pos.x = px + pw / 2 - bw / 2;
                b.pos.y = py + ph + 2; // 少し下に

                // プレイヤーと重なっていたら補正
                if (CheckSquareSquare(px, py, pw, ph, b.pos.x, b.pos.y, bw, bh))
                {
                    // プレイヤーの右に置く
                    b.pos.x = px + pw + 4;
                    b.pos.y = py;
                }

                // それでも重なれば左
                if (CheckSquareSquare(px, py, pw, ph, b.pos.x, b.pos.y, bw, bh))
                {
                    b.pos.x = px - bw - 4;
                    b.pos.y = py;
                }

                // 最後に重力ON
                b.gravity = true;
            }
            else if (IsTriggerKey(KEY_X))
            {
                b.state = BLOCK_THROW;
                b.hold = false;
                b.gravity = true;

                player.holdingBlock = nullptr;

                b.vel.x = (p->isTurn ? -6.0f : 6.0f);
                b.vel.y = -8.0f;
                SetPlayerAnimation(PLAYER_ANIM_THROW);
                g_PlayerData.isThrowing = true;


                g_PlayerData.throwAnimTimer = 15;
            }
        }
        break;
        }
    }
}


void DrawFireBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (g_FireBlock[i].active && g_FireBlock[i].handle != -1)
        {
            DrawGraph(
                (int)(g_FireBlock[i].pos.x - camera.GetX()),
                (int)(g_FireBlock[i].pos.y - camera.GetY()),
                g_FireBlock[i].handle,
                TRUE
            );
        }
    }
}


void FinFireBlock()
{
    for (int i = 0; i < BLOCK_TYPE_MAX; i++)
    {
        DeleteGraph(g_FireBlockHandle[i]);
    }
}

