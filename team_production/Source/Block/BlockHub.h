#pragma once
#include "../Player/Player.h"
#include "BlockManager.h"

extern BlockData* allBlocks[B_BLOCK_MAX];

void InitBlockHub();
void LoadBlockHub();
void StartBlockHub();
void StepBlockHub();
void UpdateBlockHub(PlayerData& player);
void DrawBlockHub();
void FinBlockHub();
extern BlockData g_Block[BLOCK_MAX];
extern BlockData g_FireBlock[BLOCK_MAX];
extern BlockData g_IceBlock[BLOCK_MAX];
extern BlockData g_IronBlock[BLOCK_MAX];
extern BlockData g_WoodBlock[BLOCK_MAX];