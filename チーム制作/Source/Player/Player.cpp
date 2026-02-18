#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"

//Player初期スポーン
#define PLAYER_DEFAULT_POS_X (0.0f)
#define PLAYER_DEFAULT_POS_Y (0.0f)

// Playerの諸々のステータス
#define PLAYER_MOVE_SPEED (4.0f)
#define PLAYER_JUMP_POWER (11.25f)
#define PLAYER_GRAVITY (0.4f)

// マップ衝突判定用のプレイヤーサイズ補正
#define PLAYER_MAP_COLLISION_OFFSET (0.05f)
// 矩形判定の位置補正
#define PLAYER_BOX_COLLISION_OFFSET_X (24)
#define PLAYER_BOX_COLLISION_OFFSET_Y (20)
// 矩形判定のサイズ
#define PLAYER_BOX_COLLISION_WIDTH (20)
#define PLAYER_BOX_COLLISION_HEIGHT (44)

PlayerData g_PlayerData = { 0 };
PlayerData g_PrevPlayerData = { 0 };

void InitPlayer()
{
    g_PlayerData.posX = PLAYER_DEFAULT_POS_X;
    g_PlayerData.posY = PLAYER_DEFAULT_POS_Y;
    g_PlayerData.moveX = 0.0f;
    g_PlayerData.moveY = 0.0f;
    g_PlayerData.active = false;
    g_PlayerData.isAir = false;

    memset(&g_PlayerData.boxCollision, 0, sizeof(g_PlayerData.boxCollision));//構造体初期化
}

void LoadPlayer()
{
    g_PlayerData.handle = LoadGraph("Data/Player/Player.png");
}

void StartPlayer()
{
    g_PlayerData.active = true;

    // 矩形判定設定
    g_PlayerData.boxCollision.posX = PLAYER_BOX_COLLISION_OFFSET_X;
    g_PlayerData.boxCollision.posY = PLAYER_BOX_COLLISION_OFFSET_Y;
    g_PlayerData.boxCollision.width = PLAYER_BOX_COLLISION_WIDTH;
    g_PlayerData.boxCollision.height = PLAYER_BOX_COLLISION_HEIGHT;
}

void StepPlayer()
{
    if (!g_PlayerData.active)
    {
        return;
    }

    g_PrevPlayerData = g_PlayerData;

    g_PlayerData.moveX = 0.0f;
    g_PlayerData.moveY += PLAYER_GRAVITY;

    if (IsInputKey(KEY_LEFT))
    {
        g_PlayerData.moveX = -PLAYER_MOVE_SPEED;
        g_PlayerData.isTurn = true;
    }
    else if (IsInputKey(KEY_RIGHT))
    {
        g_PlayerData.moveX = PLAYER_MOVE_SPEED;
        g_PlayerData.isTurn = false;
    }
}

void UpdatePlayer()
{
    if (!g_PlayerData.active)
    {
        return;
    }

    if (g_PlayerData.moveY < 0.0f || g_PlayerData.moveY > PLAYER_GRAVITY)
    {
        g_PlayerData.isAir = true;
    }
    else
    {
        if (IsTriggerKey(KEY_UP))
        {
            g_PlayerData.moveY = -PLAYER_JUMP_POWER;
        }
    }

    g_PlayerData.posX += g_PlayerData.moveX;
    g_PlayerData.posY += g_PlayerData.moveY;

}

void DrawPlayer()
{
    if (!g_PlayerData.active)
    {
        return;
    }
    DrawGraph(
        (int)(g_PlayerData.posX),
        (int)(g_PlayerData.posY),
        g_PlayerData.handle,
        TRUE
    );  
}

void FinPlayer()
{
    DeleteGraph(g_PlayerData.handle);
}

PlayerData* GetPlayer()
{
    return &g_PlayerData;
}

void PlayerHitNormalBlockX(MapChipData mapChipData)
{
    PlayerData player = g_PlayerData;
    BlockData* block = mapChipData.data;
    const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
    const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

    // ターンフラグは前回のものにしないと反転した分ずれる
    player.isTurn = g_PrevPlayerData.isTurn;

    // Y移動を戻し、横に当たっているかチェック
    player.posX = g_PlayerData.posX;
    player.posY = g_PrevPlayerData.posY;

    // 当たり判定のボックス計算
    float x, y, w, h;
    CalcBoxCollision(player, x, y, w, h);

    if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZE_OFFSET, h - SIZE_OFFSET,
        block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
    {
        // 左からあたったか
        if (player.moveX > 0.0f)
        {
            // 左に押し出す
            g_PlayerData.posX -= (x + w) - block->pos.x;
        }
        // 右からあたったか
        else if (player.moveX < 0.0f)
        {
            // 右に押し出す
            g_PlayerData.posX += (block->pos.x + MAP_CHIP_WIDTH) - x;
        }
    }
}
void PlayerHitNormalBlockY(MapChipData mapChipData)
{
    PlayerData player = g_PlayerData;
    BlockData* block = mapChipData.data;
    const float POS_OFFSET = PLAYER_MAP_COLLISION_OFFSET;
    const float SIZE_OFFSET = PLAYER_MAP_COLLISION_OFFSET * 2;

    // ターンフラグは前回のものにしないと反転した分ずれる
    player.isTurn = g_PrevPlayerData.isTurn;

    // 当たり判定のボックス計算
    float x, y, w, h;
    CalcBoxCollision(player, x, y, w, h);

    // まだ当たっているなら縦に当たっている
    if (CheckSquareSquare(x + POS_OFFSET, y + POS_OFFSET, w - SIZE_OFFSET, h - SIZE_OFFSET,
        block->pos.x, block->pos.y, MAP_CHIP_WIDTH, MAP_CHIP_HEIGHT))
    {
        // Y移動量を0にする
        g_PlayerData.moveY = 0.0f;

        // 上からあたったか
        if (player.moveY > 0.0f)
        {
            // 上に押し出す
            g_PlayerData.posY -= (y + h) - block->pos.y;
            g_PlayerData.isAir = false;
        }
        // 下からあたったか
        else if (player.moveY < 0.0f)
        {
            // 下に押し出す
            g_PlayerData.posY += (block->pos.y + MAP_CHIP_WIDTH) - y;
        }
    }
}

void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h)
{
    x = player.isTurn ?
        player.posX + PLAYER_WIDTH - player.boxCollision.posX - player.boxCollision.width :
        player.posX + player.boxCollision.posX;
    y = player.posY + player.boxCollision.posY;
    w = player.boxCollision.width;
    h = player.boxCollision.height;
}