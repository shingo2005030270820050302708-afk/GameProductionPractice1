#pragma once

#include "DxLib.h"
#include "../Player/Player.h"
#include "../Block/Block.h"
#include "../Map/MapParameter.h"

// 最大数
#define FIREFLOOR_MAX 32

struct FireFloorData
{
    bool active = false;
    int handle = -1;
    VECTOR pos = VGet(0, 0, 0);
    float width = MAP_CHIP_WIDTH;
    float height = MAP_CHIP_HEIGHT;

    // true のときプレイヤーにダメージを与える
    bool damaging = true;

    // 被覆しているブロックのインデックス（-1 = なし）
    int coverBlockIndex = -1;

    // プレイヤーに与えるダメージのクールダウン（フレーム単位）
    int damageTimer = 0;
    int damageCooldown = 60; // 調整可能
};

extern FireFloorData g_FireFloor[FIREFLOOR_MAX];
extern int g_FireFloorHandle;
extern int g_FireFloorCoolHandle;

void InitFireFloor();
void LoadFireFloor();
void StartFireFloor();
void StepFireFloor();
void UpdateFireFloor(PlayerData& player);
void DrawFireFloor();
void FinFireFloor();