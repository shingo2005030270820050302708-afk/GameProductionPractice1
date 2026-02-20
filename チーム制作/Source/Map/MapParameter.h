#pragma once
#include "DxLib.h"


#define BLOCK_MAX 40000

constexpr int MAP_CHIP_X_NUM = 64;  
constexpr int MAP_CHIP_Y_NUM = 32;
constexpr float MAP_CHIP_WIDTH = 32.0f;
constexpr float MAP_CHIP_HEIGHT = 32.0f;

enum BlockType;
enum BlockState;

enum MapChipType
{
    MAP_CHIP_NONE,
    MAP_CHIP,
    NORMAL_BLOCK,
    LEFT_BOTTOM,
    RIGHT_BOTTOM,
    LEFT_BLOCK,
    RIGHT_BLOCK,
    MIDDLE_BLOCK,
    //B_NORMAL_BLOCK,
    BLOCK_TYPE_MAX

};

struct BlockData
{
    bool active = false;
    int handle = -1;

    VECTOR pos = VGet(0, 0, 0);
    VECTOR vel = VGet(0, 0, 0);

    MapChipType mapChipType = MAP_CHIP_NONE; // Å©ïœçX
    BlockType blockType; // Å©ïœçX

    float width = MAP_CHIP_WIDTH;
    float height = MAP_CHIP_HEIGHT;

    bool gravity;
    bool hold;
    bool breakable;

    BlockState state; // Å©bool Ç∂Ç·Ç»Ç≠Çƒ enum Ç…Ç∑ÇÈ
};

struct MapChipData
{
    int mapChip = 0;       
    BlockData* data = nullptr;
};