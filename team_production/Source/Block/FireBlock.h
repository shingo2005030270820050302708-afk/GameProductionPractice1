#pragma once
#include <vector>
#include "../Player/Player.h"
#include "../Map/MapBlock.h"



// 関数
void InitFireBlock();
void LoadFireBlock();
void StartFireBlock();
void StepFireBlock();
void UpdateFireBlock(PlayerData& player);
void DrawFireBlock();
void FinFireBlock();

// ブロックとマップチップの当たり判定
void CheckBlockMapCollision(BlockData& b);

