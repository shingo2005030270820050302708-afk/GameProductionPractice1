#pragma once
#include "MapParameter.h"



extern struct BlockData g_MapBlocks[BLOCK_MAX];

void InitMapBlock();
void LoadMapBlock();
void UpdateMapBlock();
BlockData* CreateMapBlock(MapChipType type, VECTOR pos);
void DrawMapBlock();
