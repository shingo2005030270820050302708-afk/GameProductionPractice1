#pragma once
#include "MapChip.h"

// マップ管理関数
void LoadMapManager(const char* mapPath);
void StartMapManager();    // 開始処理
void UpdateMapManager();   // 更新処理
void DrawMapManager();     // 描画
void FinMapManager();  // 終了処理