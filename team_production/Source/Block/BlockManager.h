#pragma once
#include <vector>
#include "Block.h"
#include "../Map/MapParameter.h"
#include "../Map/MapManager.h"

// ブロックの状態
enum BlockState
{
    BLOCK_STAY,
    BLOCK_PUSH,
    BLOCK_LIFT,
    BLOCK_THROW,
    BREAKABLE_BLOCK
};

// ブロックの種類
enum BlockType
{
    B_NORMAL_BLOCK,
    B_IRON_BLOCK,
    B_FIRE_BLOCK,
    B_ICE_BLOCK,
    B_WOOD_BLOCK,
    B_ARROW_BLOCK,
    B_BREAKABLE_BLOCK,
    B_BLOCK_TYPE_MAX,
};

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

    //void Init();
    void SpawnBlock(int x, int y, BlockType type);
    void Update(PlayerData& player);
    void Step();
    void Draw() const;
};

extern BlockManager g_BlockManager;
