#pragma once
#include "MapParameter.h"

extern MapChipData g_MapChip[MAP_CHIP_Y_NUM][MAP_CHIP_X_NUM];

void LoadMapChipData();   // バイナリ読み込み
void CreateMap();         // ブロック生成
MapChipData GetMapChipData(int x, int y);