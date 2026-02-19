#include "BlockManager.h"

BlockManager g_BlockManager;

void BlockManager::Init()
{
    blocks.clear();

    auto map = GetMapData();

    for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
    {
        for (int x = 0; x < MAP_CHIP_X_NUM; x++)
        {
            if (map[y][x].mapChip == B_NORMAL_BLOCK)
            {
                SpawnBlock(x * MAP_CHIP_WIDTH, y * MAP_CHIP_HEIGHT);
            }
        }
    }
}

void BlockManager::SpawnBlock(int x, int y)
{
    blocks.emplace_back(x, y);
}

void BlockManager::Draw() const
{
    for (const auto& b : blocks)
        if (b.alive)
            b.Draw();
}