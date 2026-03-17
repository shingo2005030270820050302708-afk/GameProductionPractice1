#pragma once
#include "DxLib.h"
#include "../Animation/Animation.h"


#define BLOCK_MAX 20000

constexpr int MAP_CHIP_X_NUM = 64;  
constexpr int MAP_CHIP_Y_NUM = 64;
constexpr float MAP_CHIP_WIDTH = 32.0f;
constexpr float MAP_CHIP_HEIGHT = 32.0f;

// グローバル groundY の宣言（定義は .cpp に一箇所だけ置く）
extern float groundY;

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
    GOAL_BLOCK,
	COIN_BLOCK,
    NORMAL_ENEMY,
    FIRE_FLOOR_BLOCK,
    H_NORMAL_BLOCK,
	H_ICE_BLOCK,
    LASER_BLOCK,
    SWITCH_BLOCK,
    DELETE_BLOCK,
    BLOCK_TYPE_MAX,
	

};
enum BlockCategory
{
    BLOCK_CATEGORY_MAPCHIP,   // マップ用
    BLOCK_CATEGORY_OBJECT     // 持てるブロック
};

struct BlockData
{
    bool active = false;
    int handle = -1;

    VECTOR pos = VGet(0, 0, 0);
    VECTOR vel = VGet(0, 0, 0);

    // 追加：前フレーム位置（動的の連続判定/方向判定用）
    VECTOR prevPos = VGet(0, 0, 0);

    BlockCategory category = BLOCK_CATEGORY_MAPCHIP;

    MapChipType mapChipType = MAP_CHIP_NONE; // ←マップ用
    BlockType blockType;                     // ←オブジェクト用（投げ/壊れる等）

    AnimationData anim;

    float width = MAP_CHIP_WIDTH;
    float height = MAP_CHIP_HEIGHT;

    // 物理振る舞い
    bool gravity = false;  // mapchip=false / object=true
    bool hold = false;  // プレイヤーが持っている間 true
    bool breakable = false;

    // 追加：運動制御
    bool isKinematic = false; // エレベータ等「自己移動するが物理反応しない」

    // 追加：一方向足場など
    bool isOneWayPlatform = false; // 上から乗れる/下からすり抜け

    // 追加：所有情報（持ち運び）
    int holderId = -1;  // プレイヤーID 等、-1は未保持



    BlockState state;
};


struct MapChipData
{
    int mapChip = 0;       // チップ種類
    BlockData* data = nullptr;

    // スイッチで消えるフラグ
    bool disappearWithSwitch = false;
    int linkedSwitchX = -1;
    int linkedSwitchY = -1;
};