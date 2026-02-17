#pragma once
#define BLOCK_MAX 100

// ブロックの状態
enum BlockState
{
    BLOCK_STAY,
    BLOCK_LIFT,
    BLOCK_THROW,
};

// ブロックの種類
enum BlockType 
{
    NORMAL_BLOCK,
    IRON_BLOCK,
    ICE_BROCK,
    WOOD_BROCK,
    FIRE_BROCK,
    ARROW_BROCK,
    BLOCK_TYPE_MAX
};

// ブロックデータ
struct BlockData
{
    bool active;

    VECTOR pos;
    VECTOR vel;

    float w, h;

    int handle;
    BlockType type;

    BlockState state;
    bool gravity;
    bool breakable;
};
bool IsOutOfCamera(const VECTOR& pos, const CameraData& cam);

// Block.h など
bool IsNear(Player* player, BlockData* block); //距離計算
{
    float dx = p->pos.x - b->pos.x;
    float dy = p->pos.y - b->pos.y;
    return dx * dx + dy * dy < 40 * 40;
}

// カメラ
struct CameraData {
    float posX;
    float posY;
};

CameraData GetCamera();

// 関数
void InitBlock();
void LoadBlock();
void StartBlock();
void StepBlock();
void DrawBlock();
void FinBlock();

