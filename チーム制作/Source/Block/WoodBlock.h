#pragma once
#include <vector>
#include "../Player/Player.h"
#include "../Map/MapBlock.h"


// 関数
void InitWoodBlock();
void LoadWoodBlock();
void StartWoodBlock();
void StepWoodBlock();
void UpdateWoodBlock(PlayerData& player);
void DrawWoodBlock();
void FinWoodBlock();

// ブロックとマップチップの当たり判定
void CheckBlockMapCollision(BlockData& b);

// グローバル変数の宣言（他のファイルで定義されていることを想定）
extern int g_WoodBlockImg;
