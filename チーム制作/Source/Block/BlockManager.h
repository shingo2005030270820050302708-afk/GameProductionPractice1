#pragma once
#include <vector>
#include "Block.h"
#include "../Map/MapParameter.h"
#include "../Map/MapManager.h"

class Block
{
public:
    VECTOR pos;
    VECTOR vel;
    BlockType type;
    BlockState state = BLOCK_STAY;

    int handle = -1;
    bool alive = true;
    bool gravity = false;
    bool breakable = false;
    bool hold = false;

    int width = 32;
    int height = 32;

public:
    Block(int x, int y, BlockType t);

    void Update(PlayerData* player);
    void Step();
    void Draw() const;
};


struct BlockManager
{
    std::vector<Block> blocks;

    void Init();
    void SpawnBlock(int x, int y, BlockType type);
    void Update(PlayerData& player);
    void Step();
    void Draw() const;
};

extern BlockManager g_BlockManager;
