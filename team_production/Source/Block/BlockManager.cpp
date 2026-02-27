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
}

void Block::Update(PlayerData* p)
{
    if (!alive) return;
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