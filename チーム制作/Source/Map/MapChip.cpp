#include "MapChip.h"
#include "DxLib.h"
#include <cstdio>
#include "MapBlock.h"

MapChipData g_MapChip[MAP_CHIP_Y_NUM][MAP_CHIP_X_NUM] = {};

// Map.bin を読み込む
void LoadMapChipData()
{
    FILE* fp;
    if (fopen_s(&fp, "Data/Map/Map.bin", "rb") != 0 || fp == nullptr)
    {
        printfDx("マップデータ読み込み失敗\n");
        return;
    }

    int* buffer = new int[MAP_CHIP_X_NUM * MAP_CHIP_Y_NUM] {0};
    size_t readCount = fread(buffer, sizeof(int), MAP_CHIP_X_NUM * MAP_CHIP_Y_NUM, fp);
    fclose(fp);

    if (readCount != MAP_CHIP_X_NUM * MAP_CHIP_Y_NUM)
        printfDx("マップデータサイズが不正です\n");

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
            case 4:  g_MapChip[y][x].mapChip = LEFT_BLOCK; break;
            case 5:g_MapChip[y][x].mapChip = RIGHT_BLOCK; break;
            case 6:g_MapChip[y][x].mapChip = MIDDLE_BLOCK; break;
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
    for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
    {
        for (int x = 0; x < MAP_CHIP_X_NUM; x++)
        {
            MapChipType type = static_cast<MapChipType>(g_MapChip[y][x].mapChip);
            if (type == MAP_CHIP_NONE) continue;

            VECTOR pos = VGet(x * MAP_CHIP_WIDTH, y * MAP_CHIP_HEIGHT, 0.0f);
            BlockData* block = CreateBlock(type, pos);

            if (block != nullptr)
                g_MapChip[y][x].data = block;
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