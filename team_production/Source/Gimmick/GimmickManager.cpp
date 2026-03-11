#include "GimmickManager.h"
#include "../Collision/Collision.h"
#include "FireFloor.h"


void GimmickManager::InitGimmickManager()
{
	InitFireFloor();
}

void GimmickManager::LoadGimmickManager()
{
	LoadFireFloor();
}

void GimmickManager::StartGimmickManager()
{
	StartFireFloor();
}

void GimmickManager::StepGimmickManager()
{
	StepFireFloor();
}

void GimmickManager::UpdateGimmickManager(PlayerData& player)
{
	UpdateFireFloor(player);
}

void GimmickManager::DrawGimmickManager()
{
	DrawFireFloor();
}

void GimmickManager::FinGimmickManager()
{
	FinFireFloor();
}