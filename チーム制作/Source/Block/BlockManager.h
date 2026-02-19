#pragma once
#include <vector>
#include "Block.h"
#include "../Map/MapParameter.h"
#include "../Map/MapManager.h"

struct BlockManager
{
    std::vector<Block> blocks;

    void Init();
    void SpawnBlock(int x, int y);
    void Draw() const;
};

// グローバルインスタンス
extern BlockManager g_BlockManager;