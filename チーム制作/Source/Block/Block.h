#pragma once
#include <vector>
#include "../Player/Player.h"
#include "../Map/MapBlock.h"

// ブロックの状態
enum BlockState
{
    BLOCK_STAY,
    BLOCK_LIFT,
    BLOCK_THROW,
    BREAKABLE_BLOCK
};

// ブロックの種類
enum BlockType 
{
    B_NORMAL_BLOCK,
    B_IRON_BLOCK,
    B_ICE_BLOCK,
    B_WOOD_BLOCK,
    B_FIRE_BLOCK,
    B_ARROW_BLOCK,
};

// 関数
void InitBlock();
void LoadBlock();
void StartBlock();
void StepBlock();
void UpdateBlock(PlayerData& player);
void DrawBlock();
void FinBlock();

// ブロックとマップチップの当たり判定
void CheckBlockMapCollision(BlockData& b);

