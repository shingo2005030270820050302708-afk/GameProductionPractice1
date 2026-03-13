#pragma once

#include "DxLib.h"
#include "../Player/Player.h"
#include "../Block/Block.h"
#include "../Map/MapParameter.h"

// ç≈ëÂêî
#define FIREFLOOR_MAX 32

struct FireFloorData
{
    bool active = false;
    int handle = -1;
    VECTOR pos = VGet(0, 0, 0);
    float width = MAP_CHIP_WIDTH;
    float height = MAP_CHIP_HEIGHT;

    bool damaging = true;
    int coverBlockIndex = -1;
    int damageTimer = 0;
    int damageCooldown = 60;
};

extern FireFloorData g_FireFloor[FIREFLOOR_MAX];
extern int g_FireFloorHandle;
extern int g_FireFloorCoolHandle;

// í«â¡ÅFCreateFireFloor ÇêÈåæ
FireFloorData* CreateFireFloorIfNotExist(VECTOR pos);

FireFloorData* CreateFireFloor(VECTOR pos);

void InitFireFloor();
void LoadFireFloor();
void StartFireFloor();
void StepFireFloor();
void UpdateFireFloor(PlayerData& player);
void DrawFireFloor();
void FinFireFloor();