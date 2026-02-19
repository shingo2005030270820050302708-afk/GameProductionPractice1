#include "DxLib.h"
#include "MapParameter.h"
#include "MapBlock.h"
#include "../../Data/Camera/Camera.h"

BlockData g_MapBlocks[BLOCK_MAX];
int g_MapBlocksHandle[BLOCK_TYPE_MAX] = { -1 };


void InitMapBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++) {
        g_MapBlocks[i].active = false;
        g_MapBlocks[i].handle = -1;
        g_MapBlocks[i].mapChipType = MAP_CHIP;
        g_MapBlocks[i].pos = VGet(0, 0, 0);
        g_MapBlocks[i].width = MAP_CHIP_WIDTH;
        g_MapBlocks[i].height = MAP_CHIP_HEIGHT;
    }
}

// グラフィック読み込み
void LoadMapBlock()
{
    g_MapBlocksHandle[NORMAL_BLOCK] = LoadGraph("Data/Map/Tiles.png"); // ノーマルブロック画像
    g_MapBlocksHandle[LEFT_BOTTOM] = LoadGraph("Data/Map/IndustrialTile_16.png"); //左角のブロック画像
    g_MapBlocksHandle[RIGHT_BOTTOM] = LoadGraph("Data/Map/IndustrialTile_17.png"); //右角のブロック画像
    g_MapBlocksHandle[LEFT_BLOCK] = LoadGraph("Data/Map/Left_Block.png"); //左のブロック画像
    g_MapBlocksHandle[RIGHT_BLOCK] = LoadGraph("Data/Map/Right_Block.png"); //左のブロック画像
    g_MapBlocksHandle[MIDDLE_BLOCK] = LoadGraph("Data/Map/M_Block.png"); //中央のブロック画像
}
// ブロック生成
BlockData* CreateMapBlock(MapChipType type, VECTOR pos)
{
    for (int i = 0; i < BLOCK_MAX; i++) {
        if (!g_MapBlocks[i].active) {
            g_MapBlocks[i].active = true;
            // MapChipType → BlockType に変換
            g_MapBlocks[i].mapChipType = MAP_CHIP;
            g_MapBlocks[i].pos = pos;
            g_MapBlocks[i].width = MAP_CHIP_WIDTH;
            g_MapBlocks[i].height = MAP_CHIP_HEIGHT;

            // タイプごとに画像ハンドルを設定
            if (type >= 0 && type < BLOCK_TYPE_MAX)
                g_MapBlocks[i].handle = g_MapBlocksHandle[type];
            else
                g_MapBlocks[i].handle = -1;

            return &g_MapBlocks[i];
        }
    }
    return nullptr;
}
// 描画


extern Camera camera;
void DrawMapBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (g_MapBlocks[i].active && g_MapBlocks[i].handle != -1)
        {
            DrawGraph(
                (int)(g_MapBlocks[i].pos.x - camera.GetX()),
                (int)(g_MapBlocks[i].pos.y - camera.GetY()),
                g_MapBlocks[i].handle,
                TRUE
            );
        }
    }
}
//いじらない

