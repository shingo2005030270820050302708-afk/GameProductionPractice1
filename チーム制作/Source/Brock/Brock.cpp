#include "Player.h"
#include "Enemy.h"
#include "Camera.h" 
#include <DxLib.h>
#include "Brock.h"


BlockData g_Block[BLOCK_MAX];
int g_BlockHandle[BLOCK_TYPE_MAX] = { 0 };
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
        g_Block[i].w = 32;
        g_Block[i].h = 32;
    }
}
void LoadBlock()
{
    g_BlockHandle[NORMAL_BLOCK] = LoadGraph("Data/Block/Normal.png");
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
            g_Block[i].type = type;
            g_Block[i].pos = pos;
            g_Block[i].vel = VGet(0, 0, 0);
            g_Block[i].state = BLOCK_STAY;
            g_Block[i].gravity = false;
            g_Block[i].handle = g_BlockHandle[type];
            g_Block[i].breakable = (type == BREAKABLE_BLOCK);
            
            return &g_Block[i];
        }
    }

    // 空きがなかった
    return nullptr;
}

void StepBlock(Player* player)
{
    CameraData cam = GetCamera();

    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (IsOutOfCamera(g_Block[i].pos, cam))
        {
            g_Block[i].active = false;
            continue;
        }
        switch (g_Block[i].state)
        {
        case BLOCK_LIFT:
            // プレイヤーに追従（物理じゃないけど Step に置く）
            g_Block[i].pos.x = player->pos.x;
            g_Block[i].pos.y = player->pos.y - player->h;
            break;

        case BLOCK_THROW:
            // 重力
            if (g_Block[i].gravity)
                g_Block[i].vel.y += 0.5f;

            // 移動
            g_Block[i].pos.x += g_Block[i].vel.x;
            g_Block[i].pos.y += g_Block[i].vel.y;
            break;
        }

        if (IsOutOfCamera(g_Block[i].pos, cam))
        {
            g_Block[i].active = false;
            continue;
        }
        {
            g_Block[i].active = false;
        }
    }
}
void UpdateBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!g_Block[i].active)
            continue;

        // 状態遷移だけを書く
        switch (g_Block[i].state)
        {
        case BLOCK_STAY:
            // 近くて Z → 持つ
            if (IsNear(player, &g_Block[i]) && CheckHitKey(KEY_INPUT_Z))
            {
                // すでに持ってたら拾えない
                if (player->holdBlock != nullptr)
                    continue;

                g_Block[i].state = BLOCK_LIFT;
                g_Block[i].gravity = false;
                g_Block[i].vel = VGet(0, 0, 0);

                player->holdBlock = &g_Block[i];
            }
            break;

        case BLOCK_LIFT:
            // 下＋Z → 置く
            if (CheckHitKey(KEY_INPUT_DOWN) && CheckHitKey(KEY_INPUT_Z))
            {
                g_Block[i].state = BLOCK_STAY;
                g_Block[i].gravity = false;
                player->holdBlock = nullptr;
            }

            else if (CheckHitKey(KEY_INPUT_Z))
            {
                g_Block[i].state = BLOCK_THROW;
                g_Block[i].gravity = true;
                g_Block[i].vel.x = 6.0f * player->dir;
                g_Block[i].vel.y = -8.0f;

                player->holdBlock = nullptr;
            }

        case BLOCK_THROW:
        {
            // 重力
            if (g_Block[i].gravity)
                g_Block[i].vel.y += 0.5f;

            // 移動
            g_Block[i].pos.x += g_Block[i].vel.x;
            g_Block[i].pos.y += g_Block[i].vel.y;

            // ★ 敵との当たり判定（ダメージ or ブロック破壊）
            for (int i = 0; i < ENEMY_MAX; i++)
            {
                if (!g_Enemy[i].active)
                    continue;

                // 当たり判定（矩形 or 円形）
                if (CheckHitBlockEnemy(&g_Block[i], &g_Enemy[i]))
                {
                    // 敵にダメージ
                    g_Enemy[i].hp -= 1;

                    // ブロックが壊れるタイプなら壊す
                    if (g_Block[i].breakable)
                    {
                        g_Block[i].active = false;
                    }
                    else
                    {
                        // 壊れないなら落下状態に戻す
                        g_Block[i].state = BLOCK_STAY;
                        g_Block[i].gravity = true;
                    }

                    break; // 1体に当たったら終了
                }
            }
        }
        break;
            break;
        }
    }
}
      
    void DrawBlock()
    {
        CameraData cam = GetCamera();

        for (int i = 0; i < BLOCK_MAX; i++)
        {
            BlockData& b = g_Block[i];
            if (!b.active) continue;

            DrawGraph(
                (int)(b.pos.x - cam.posX),
                (int)(b.pos.y - cam.posY),
                b.handle,
                TRUE
            );
        }
    }
    void FinBlock()
    {
        for (int i = 0; i < BLOCK_TYPE_MAX; i++)
        {
            DeleteGraph(g_BlockHandle[i]);
        }
    }

    BlockData* GetBlocks()
    {
        return g_Block;
    }


