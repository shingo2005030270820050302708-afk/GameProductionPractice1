#include "BlockManager.h"
#include <DxLib.h>

extern int g_BlockHandle[];

// Block ƒNƒ‰ƒX‚ÌŽÀ‘•
Block::Block(int x, int y, BlockType t)
{
    pos = VGet((float)x, (float)y, 0);
    vel = VGet(0, 0, 0);
    type = t;

    handle = g_BlockHandle[t];
    breakable = (t == BREAKABLE_BLOCK);
}

void Block::Step()
{
    if (!alive) return;

    switch (state)
    {
    case BLOCK_THROW:
        if (gravity)
            vel.y += 0.5f;

        pos.x += vel.x;
        pos.y += vel.y;
        break;
    }
}

void Block::Update(PlayerData* p)
{
    if (!alive) return;

    switch (state)
    {
    case BLOCK_STAY:
    {
        float px = p->posX + 10;
        float py = p->posY + 10;
        float pw = 30;
        float ph = 40;

        float bx = pos.x;
        float by = pos.y;

        bool hit =
            (px < bx + width) &&
            (px + pw > bx) &&
            (py < by + height) &&
            (py + ph > by);
        
        printfDx("Block‚Ó‚ê‚ ‚¢\n");
        if (hit && CheckHitKey(KEY_INPUT_X))
        {
            state = BLOCK_LIFT;
            gravity = false;
            vel = VGet(0, 0, 0);
            hold = true;
        }
    }
    break;

    case BLOCK_LIFT:
        pos.x = p->posX;
        pos.y = p->posY - PLAYER_HEIGHT;

        if (CheckHitKey(KEY_INPUT_DOWN) && CheckHitKey(KEY_INPUT_X))
        {
            hold = false;
            state = BLOCK_STAY;
        }
        else if (CheckHitKey(KEY_INPUT_X))
        {
            gravity = true;
            hold = false;
            state = BLOCK_THROW;
            vel.x = (p->isTurn ? -6.0f : 6.0f);
            vel.y = -8.0f;
        }
        break;
    }
}

void Block::Draw() const
{
    if (!alive || handle == -1) return;

    DrawGraph((int)pos.x, (int)pos.y, handle, TRUE);
}

// BlockManager ‚ÌŽÀ‘•
void BlockManager::SpawnBlock(int x, int y, BlockType type)
{
    blocks.emplace_back(x, y, type);
}

void BlockManager::Update(PlayerData& player)
{
    for (auto& b : blocks)
        b.Update(&player);
}

void BlockManager::Step()
{
    for (auto& b : blocks)
        b.Step();
}

void BlockManager::Draw() const
{
    for (const auto& b : blocks)
        b.Draw();
}