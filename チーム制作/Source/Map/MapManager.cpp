#include "MapManager.h"
#include "DxLib.h"
#include "MapBlock.h"
#include "../Player/Player.h"

// マップ読み込み
void LoadMapManager()
{
    LoadMapBlock();        // タイル画像読み込み
    LoadMapChipData();  // Map.bin 読み込み
    CreateMap();        // ブロック生成
}
// 開始処理（今は空）
void StartMapManager()
{
}

// 更新処理（今は空）
void UpdateMapManager()
{
    CheckPlayerMapCollision();
}

// 描画
void DrawMapManager()
{
    DrawMapBlock();
}

// 終了処理
void FinMapManager()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        g_MapBlocks[i].active = false;
    }
}
//いじらない