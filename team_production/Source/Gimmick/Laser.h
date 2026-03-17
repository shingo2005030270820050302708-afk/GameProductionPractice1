#pragma once

#include "DxLib.h"
#include "../Player/Player.h"
#include "../Block/Block.h"
#include "../Map/MapParameter.h"

// 最大レーザー数
#define LASER_MAX 32

struct LaserData
{
    bool active = false;
    int handle = -1;
    VECTOR start = VGet(0, 0, 0);  // レーザー始点
    VECTOR end = VGet(0, 0, 0);    // レーザー終点（衝突で更新）
    float width = 10.0f;           // 幅10px

    bool damaging = true;           // プレイヤーにダメージを与えるか

    int coverBlockIndex = -1;       // 持ちブロックがレーザーにかかっている場合の無効化用
};

// グローバル配列
extern LaserData g_Laser[LASER_MAX];
extern int g_LaserHandle;

// 作成
LaserData* CreateLaserIfNotExist(VECTOR start, float length);
LaserData* CreateLaser(VECTOR start, float length);

// 初期化 / 読み込み / 更新 / 描画 / 終了
void InitLaser();
void LoadLaser();
void StartLaser();
void StepLaser();
void UpdateLaser(PlayerData& player);
void DrawLaser();
void FinLaser();