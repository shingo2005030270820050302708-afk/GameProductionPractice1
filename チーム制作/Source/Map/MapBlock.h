#pragma once
#include "MapParameter.h"


extern struct BlockData g_Blocks[BLOCK_MAX];

void InitBlock();
void LoadBlock();
BlockData* CreateBlock(MapChipType type, VECTOR pos);
void DrawBlock();
