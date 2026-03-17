#include "MapChip.h"
#include "DxLib.h"
#include <cstdio>
#include "MapBlock.h"
#include "../Enemy/NormalEnemy.h"
#include "../Gimmick/FireFloor.h"
#include "../Block/BlockManager.h"
#include "../Block/IceBlock.h"
#include "../Gimmick/Laser.h"
#include "../Gimmick/Switch.h"

MapChipData g_MapChip[MAP_CHIP_Y_NUM][MAP_CHIP_X_NUM] = {};


MapChipData(*GetMapData())[MAP_CHIP_X_NUM]
{
    return g_MapChip;
}

// Map.bin を読み込む
// 引数 path を受け取るように変更
void LoadMapChipData(const char* path)
{
    FILE* fp;
    if (fopen_s(&fp, path, "rb") != 0 || fp == nullptr)
    {
        printfDx("マップデータ読み込み失敗: %s\n", path);
        return;
    }

    int* buffer = new int[MAP_CHIP_X_NUM * MAP_CHIP_Y_NUM] { 0 };
    size_t readCount = fread(buffer, sizeof(int), MAP_CHIP_X_NUM * MAP_CHIP_Y_NUM, fp);
    fclose(fp);

    if (readCount != MAP_CHIP_X_NUM * MAP_CHIP_Y_NUM)
        printfDx("マップデータサイズが不正です: %s\n", path);

    for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
    {
        for (int x = 0; x < MAP_CHIP_X_NUM; x++)
        {
            int val = buffer[y * MAP_CHIP_X_NUM + x];
            switch (val)
            {
            case 1: g_MapChip[y][x].mapChip = NORMAL_BLOCK; break;
            case 2: g_MapChip[y][x].mapChip = LEFT_BOTTOM;  break;
            case 3: g_MapChip[y][x].mapChip = RIGHT_BOTTOM; break;
            case 4: g_MapChip[y][x].mapChip = LEFT_BLOCK; break;
            case 5: g_MapChip[y][x].mapChip = RIGHT_BLOCK; break;
            case 6: g_MapChip[y][x].mapChip = MIDDLE_BLOCK; break;
            case 7: g_MapChip[y][x].mapChip = GOAL_BLOCK; break;
			case 8: g_MapChip[y][x].mapChip = COIN_BLOCK; break;
            case 9: g_MapChip[y][x].mapChip = NORMAL_ENEMY; break;
            case 10: g_MapChip[y][x].mapChip = FIRE_FLOOR_BLOCK; break;
            case 11: g_MapChip[y][x].mapChip = H_NORMAL_BLOCK; break;
            case 12: g_MapChip[y][x].mapChip = H_ICE_BLOCK; break;
            case 13: g_MapChip[y][x].mapChip = LASER_BLOCK; break;
            case 14: g_MapChip[y][x].mapChip = SWITCH_BLOCK; break;
            default: g_MapChip[y][x].mapChip = MAP_CHIP_NONE; break;
            }
            g_MapChip[y][x].data = nullptr;
        }
    }

    delete[] buffer;
}

// 配列からブロック生成
void CreateMap()
{
    int enemyIndex = 0;

    for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
    {
        for (int x = 0; x < MAP_CHIP_X_NUM; x++)
        {
            MapChipType type = static_cast<MapChipType>(g_MapChip[y][x].mapChip);
            VECTOR pos = VGet(x * MAP_CHIP_WIDTH, y * MAP_CHIP_HEIGHT, 0.0f);

            // 敵チップなら敵を生成
            if (type == NORMAL_ENEMY)
            {
                if (enemyIndex < ENEMY_MAX)
                {
                    NormalEnemyData& e = g_NormalEnemyData[enemyIndex];

                    e.active = true;
                    e.state = Move;
                    e.gravity = true;
                    e.pos = pos;
                    e.vel = VGet(0, 0, 0);
                    e.boxCollision.width = 45.0f;
                    e.boxCollision.height = 45.0f;

                    enemyIndex++;
                }

                // 敵チップはブロックを生成しないので continue
                continue;
            }

            if (type == FIRE_FLOOR_BLOCK)
            {
                CreateFireFloorIfNotExist(pos); // 二重登録防止
                continue;
            }
            if (type == LASER_BLOCK)
            {
                VECTOR pos = VGet(x * MAP_CHIP_WIDTH + MAP_CHIP_WIDTH / 2.0f, y * MAP_CHIP_HEIGHT, 0.0f);
                float length = MAP_CHIP_HEIGHT * MAP_CHIP_Y_NUM; // 最大長さ、下に向かって伸びる
                CreateLaserIfNotExist(pos, length); // 二重登録防止
                continue;
            }
            if (type == SWITCH_BLOCK)
            {
                int mapX = x;
                int mapY = y;

                // 既に LoadSwitch() でロード済みの画像を使う
                extern SwitchData g_Switch[SWITCH_MAX]; // もしまだ extern が必要なら
                int handleNormal = g_Switch[0].handleNormal;
                int handlePressed = g_Switch[0].handlePressed;

                // blockIndex はまだ決まっていない場合は -1
                CreateSwitch(mapX, mapY, -1, handleNormal, handlePressed);

                // スイッチに紐付く DELETE_BLOCK の位置を登録
                for (int by = 0; by < MAP_CHIP_Y_NUM; by++)
                {
                    for (int bx = 0; bx < MAP_CHIP_X_NUM; bx++)
                    {
                        if (g_MapChip[by][bx].mapChip == DELETE_BLOCK)
                        {
                            g_MapChip[by][bx].linkedSwitchX = mapX;
                            g_MapChip[by][bx].linkedSwitchY = mapY;
                        }
                    }
                }

                continue;
            }
            if (type == H_NORMAL_BLOCK)
            {
                CreateBlock(B_NORMAL_BLOCK, pos);
                continue;
            }

            if (type == H_ICE_BLOCK)
            {
                CreateIceBlock(B_ICE_BLOCK, pos);
                continue;
            }

            // ブロック生成
            if (type != MAP_CHIP_NONE)
            {
                BlockData* block = CreateMapBlock(type, pos);
                if (block != nullptr)
                    g_MapChip[y][x].data = block;
            }
        }
    }
}

// 範囲外は空マスとして返す
MapChipData GetMapChipData(int x, int y)
{
    if (x < 0 || x >= MAP_CHIP_X_NUM || y < 0 || y >= MAP_CHIP_Y_NUM)
    {
        MapChipData empty;
        empty.mapChip = MAP_CHIP_NONE;
        empty.data = nullptr;
        return empty;
    }
    return g_MapChip[y][x];
}
////