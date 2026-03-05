#include "BlockHub.h"
#include "Block.h"
#include"FireBlock.h"
#include "IceBlock.h"
#include "IronBlock.h"
#include "WoodBlock.h"
#include "BlockManager.h"

void InitBlockHub()
{
	InitBlock();
	InitFireBlock();
	InitIceBlock();
	InitIronBlock();
	InitWoodBlock();
}

void LoadBlockHub()
{
	LoadBlock();
	LoadFireBlock();
	LoadIceBlock();
	LoadIronBlock();
	LoadWoodBlock();
}

void StartBlockHub()
{
	StartBlock();
	StartFireBlock();
	StartIceBlock();
	StartIronBlock();
	StartWoodBlock();
}


void StepBlockHub()
{
    StepBlock();
    StepFireBlock();
    StepIceBlock();
    StepIronBlock();
    StepWoodBlock();


    BlockData* allBlocks[B_BLOCK_MAX];
    int allCount = 0;

    // 通常ブロック
    for (int i = 0; i < BLOCK_MAX; i++)
        if (g_Block[i].active) allBlocks[allCount++] = &g_Block[i];

    // Fireブロック
    for (int i = 0; i < BLOCK_MAX; i++)
        if (g_FireBlock[i].active) allBlocks[allCount++] = &g_FireBlock[i];

    // Iceブロック
    for (int i = 0; i < BLOCK_MAX; i++)
        if (g_IceBlock[i].active) allBlocks[allCount++] = &g_IceBlock[i];

    // Ironブロック
    for (int i = 0; i < BLOCK_MAX; i++)
        if (g_IronBlock[i].active) allBlocks[allCount++] = &g_IronBlock[i];

    // Woodブロック
    for (int i = 0; i < BLOCK_MAX; i++)
        if (g_WoodBlock[i].active) allBlocks[allCount++] = &g_WoodBlock[i];

    // 安全チェック（B_BLOCK_MAXを超えないか）
    if (allCount > B_BLOCK_MAX)
    {
        allCount = B_BLOCK_MAX; // 超えた分は無視
        // ここでログや警告出すのも可
    }

    // 衝突判定：ブロック同士
    for (int i = 0; i < allCount; i++)
    {
        BlockData* a = allBlocks[i];
        if (a->state == BLOCK_LIFT) continue;

        for (int j = i + 1; j < allCount; j++)
        {
            BlockData* b = allBlocks[j];
            if (b->state == BLOCK_LIFT) continue;
            if (a == b) continue;

            if (CheckSquareSquare(a->pos.x, a->pos.y, a->width, a->height,
                b->pos.x, b->pos.y, b->width, b->height))
            {
                ResolveBlockCollision(*a, *b);
            }
        }
    }

    // 衝突判定：マップブロック
    for (int i = 0; i < allCount; i++)
    {
        BlockData* moving = allBlocks[i];
        if (!moving->active) continue;

        for (int j = 0; j < BLOCK_MAX; j++)
        {
            BlockData* mapBlock = &g_MapBlocks[j];
            if (!mapBlock->active) continue;

            if (CheckSquareSquare(
                moving->pos.x, moving->pos.y, moving->width, moving->height,
                mapBlock->pos.x, mapBlock->pos.y, mapBlock->width, mapBlock->height))
            {
                ResolveBlockMapCollision(*moving, *mapBlock);
            }
        }
    }
}
void UpdateBlockHub(PlayerData& player)
{
	UpdateBlock(player);
	UpdateFireBlock(player);
	UpdateIceBlock(player);
	UpdateIronBlock(player);
	UpdateWoodBlock(player);
}

void DrawBlockHub()
{
	DrawBlock();
	DrawFireBlock();
	DrawIceBlock();
	DrawIronBlock();
	DrawWoodBlock();
}

void FinBlockHub()
{
	FinBlock();
	FinFireBlock();
	FinIceBlock();
	FinIronBlock();
	FinWoodBlock();
}
