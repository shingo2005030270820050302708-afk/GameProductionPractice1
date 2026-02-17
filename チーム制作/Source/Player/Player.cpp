#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"

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
    g_PlayerData.isTurn = false;
    g_PlayerData.active = false;
}

void LoadPlayer()
{
    g_PlayerData.handle = LoadGraph("Data/Player/Player.png");
}

void StartPlayer()
{
    g_PlayerData.active = true;
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

    if (IsInputKey(KEY_UP))
    {
        g_PlayerData.moveY = PLAYER_JUMP_POWER;
    }
    else if (IsInputKey(KEY_LEFT))
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
    g_PlayerData.posX += g_PlayerData.moveX;
    g_PlayerData.posY += g_PlayerData.moveY;
}

void DrawPlayer()
{
    if (!g_PlayerData.active) return;

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

void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h)
{
    x = player.isTurn ?
        player.posX + PLAYER_WIDTH - player.boxCollision.posX - player.boxCollision.width :
        player.posX + player.boxCollision.posX;
    y = player.posY + player.boxCollision.posY;
    w = player.boxCollision.width;
    h = player.boxCollision.height;
}