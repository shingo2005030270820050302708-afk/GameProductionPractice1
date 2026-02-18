#pragma once
#include "MapParameter.h"


extern struct BlockData g_Blocks[BLOCK_MAX];

void InitMapBlock();
void LoadMapBlock();
BlockData* CreateMapBlock(MapChipType type, VECTOR pos);
void DrawMapBlock();
