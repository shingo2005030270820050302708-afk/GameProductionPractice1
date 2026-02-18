#include "DxLib.h"
#include "MapParameter.h"
#include "MapBlock.h"

BlockData g_Blocks[BLOCK_MAX];
int g_BlockHandle[BLOCK_TYPE_MAX] = { -1 };


void InitBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++) {
        g_Blocks[i].active = false;
        g_Blocks[i].handle = -1;
        g_Blocks[i].mapChipType = MAP_CHIP_NONE;
        g_Blocks[i].pos = VGet(0, 0, 0);
        g_Blocks[i].width = MAP_CHIP_WIDTH;
        g_Blocks[i].height = MAP_CHIP_HEIGHT;
    }
}

// グラフィック読み込み
void LoadBlock()
{
    g_BlockHandle[NORMAL_BLOCK] = LoadGraph("Data/Map/Tiles.png"); // ノーマルブロック画像
    g_BlockHandle[LEFT_BOTTOM] = LoadGraph("Data/Map/IndustrialTile_16.png"); //左角のブロック画像
    g_BlockHandle[RIGHT_BOTTOM] = LoadGraph("Data/Map/IndustrialTile_17.png"); //右角のブロック画像
    g_BlockHandle[LEFT_BLOCK] = LoadGraph("Data/Map/Left_Block.png"); //左のブロック画像
    g_BlockHandle[RIGHT_BLOCK] = LoadGraph("Data/Map/Right_Block.png"); //左のブロック画像
    g_BlockHandle[MIDDLE_BLOCK] = LoadGraph("Data/Map/M_Block.png"); //中央のブロック画像
}
// ブロック生成
BlockData* CreateBlock(MapChipType type, VECTOR pos)
{
    for (int i = 0; i < BLOCK_MAX; i++) {
        if (!g_Blocks[i].active) {
            g_Blocks[i].active = true;
            // MapChipType → BlockType に変換
            g_Blocks[i].blockType = BlockType();
            g_Blocks[i].pos = pos;
            g_Blocks[i].width = MAP_CHIP_WIDTH;
            g_Blocks[i].height = MAP_CHIP_HEIGHT;

            // タイプごとに画像ハンドルを設定
            if (type >= 0 && type < BLOCK_TYPE_MAX)
                g_Blocks[i].handle = g_BlockHandle[type];
            else
                g_Blocks[i].handle = -1;

            return &g_Blocks[i];
        }
    }
    return nullptr;
}
// 描画
void DrawBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++) {
        if (g_Blocks[i].active && g_Blocks[i].handle != -1) {
            DrawGraph((int)g_Blocks[i].pos.x, (int)g_Blocks[i].pos.y, g_Blocks[i].handle, TRUE);
        }
    }
}

