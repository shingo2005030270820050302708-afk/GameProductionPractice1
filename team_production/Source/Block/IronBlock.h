#pragma once
#include <vector>
#include "../Player/Player.h"
#include "../Map/MapBlock.h"

// 関数
void InitIronBlock();
void LoadIronBlock();
void StartIronBlock();
void StepIronBlock();
void UpdateIronBlock(PlayerData& player);
void DrawIronBlock();
void FinIronBlock();

// ブロックとマップチップの当たり判定
void CheckBlockMapCollision(BlockData& b);

