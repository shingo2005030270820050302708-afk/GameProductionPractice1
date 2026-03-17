#pragma once
#include "DxLib.h"
#include "../Player/Player.h"
#include "../Map/MapBlock.h"

#define SWITCH_MAX 32

struct SwitchData
{
    VECTOR pos;           // スイッチの座標
    float width, height;  // サイズ
    bool active;          // スイッチ自体の有効・描画
    bool pressed;         // 押されているか
    int linkedMapBlockIndex; // 押されたときに出現させるマップブロックのID

    int handleNormal;     // 通常画像
    int handlePressed;    // 押されている画像
};
// スイッチ配列
extern SwitchData g_Switch[SWITCH_MAX];

// 初期化・読み込み
void InitSwitch();
void LoadSwitch();  // 画像読み込み

// 更新・描画
void UpdateSwitches(PlayerData& player);
void DrawSwitches();

// Switch.h 側に追加
SwitchData* CreateSwitch(int mapX, int mapY, int blockIndex,
    int handleNormal, int handlePressed);