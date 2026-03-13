#pragma once
#include <vector>
#include "../Player/Player.h"
#include "../Map/MapBlock.h"

extern BlockData g_Block[BLOCK_MAX];

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
bool IsBlockOnAnotherBlock(const BlockData& block, BlockData* blockArray, int blockMax);

BlockData* CreateBlock(BlockType type, VECTOR pos);



