#include "BlockHub.h"
#include "Block.h"
#include"FireBlock.h"
#include "IceBlock.h"
#include "IronBlock.h"
#include "WoodBlock.h"

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
