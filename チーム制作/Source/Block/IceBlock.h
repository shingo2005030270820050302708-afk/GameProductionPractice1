#pragma once
#include <vector>
#include "../Player/Player.h"
#include "../Map/MapBlock.h"


// 関数
void InitIceBlock();
void LoadIceBlock();
void StartIceBlock();
void StepIceBlock();
void UpdateIceBlock(PlayerData& player);
void DrawIceBlock();
void FinIceBlock();

// ブロックとマップチップの当たり判定
void CheckBlockMapCollision(BlockData& b);

