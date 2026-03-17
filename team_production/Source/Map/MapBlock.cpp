#include "DxLib.h"
#include "MapParameter.h"
#include "MapBlock.h"
#include "../../Data/Camera/Camera.h"
#include "../Block/Block.h"
#include "../Scene/GlobalData.h"

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
    g_MapBlocksHandle[GOAL_BLOCK] = LoadGraph("Data/Map/Goal.png"); //中央のブロック画像
	g_MapBlocksHandle[COIN_BLOCK] = LoadGraph("Data/Map/golden.rotate.png"); //コインブロック画像
    g_MapBlocksHandle[DELETE_BLOCK] = LoadGraph("Data/Map/IndustrialTile_27.png");
}
// ブロック生成
BlockData* CreateMapBlock(MapChipType type, VECTOR pos)
{
    for (int i = 0; i < BLOCK_MAX; i++) {
        if (!g_MapBlocks[i].active) {

            g_MapBlocks[i].active = true;
            g_MapBlocks[i].mapChipType = type;
            g_MapBlocks[i].pos = pos;
            g_MapBlocks[i].width = MAP_CHIP_WIDTH;
            g_MapBlocks[i].height = MAP_CHIP_HEIGHT;

            if (type >= 0 && type < BLOCK_TYPE_MAX)
                g_MapBlocks[i].handle = g_MapBlocksHandle[type];
            if (type == COIN_BLOCK)
            {
                g_MapBlocks[i].anim.handle = g_MapBlocksHandle[COIN_BLOCK];

                StartAnimation(
                    &g_MapBlocks[i].anim,
                    pos.x,
                    pos.y,
                    6,      // アニメ速度
                    14,      // フレーム数
                    32,     // 1コマ幅
                    32,
                    true
                );
            }

            return &g_MapBlocks[i];
        }
    }
    return nullptr;
}// 描画

void UpdateMapBlock()
{
    PlayerData* p = GetPlayer();

    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!g_MapBlocks[i].active) continue;

        if (g_MapBlocks[i].mapChipType == COIN_BLOCK)
        {
            g_MapBlocks[i].anim.posX = g_MapBlocks[i].pos.x;
            g_MapBlocks[i].anim.posY = g_MapBlocks[i].pos.y;

            UpdateAnimation(&g_MapBlocks[i].anim);

            // コイン取得判定
            if (CheckSquareSquare(
                p->posX,
                p->posY,
                p->boxCollision.width,
                p->boxCollision.height,

                g_MapBlocks[i].pos.x,
                g_MapBlocks[i].pos.y,
                g_MapBlocks[i].width,
                g_MapBlocks[i].height
            ))
            {
                g_MapBlocks[i].active = false;
                gData.coin++;
            }
        }
    }
}

extern Camera camera;
void DrawMapBlock()
{
    for (int i = 0; i < BLOCK_MAX; i++)
    {
        if (!g_MapBlocks[i].active) continue;

        if (g_MapBlocks[i].mapChipType == COIN_BLOCK)
        {
            DrawAnimation(
                &g_MapBlocks[i].anim,
                g_MapBlocks[i].pos.x - camera.GetX(),
                g_MapBlocks[i].pos.y - camera.GetY()
            );
        }
        else
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

