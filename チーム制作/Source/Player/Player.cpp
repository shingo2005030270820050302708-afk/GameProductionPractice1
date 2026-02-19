#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"
#include "../Map/MapChip.h"

//Player初期スポーン
#define PLAYER_DEFAULT_POS_X (400.0f)
#define PLAYER_DEFAULT_POS_Y (0.0f)

// Playerの諸々のステータス
#define PLAYER_MOVE_SPEED (2.0f)
#define PLAYER_JUMP_POWER (10.0f)
#define PLAYER_GRAVITY (0.4f)

// マップ衝突判定用のプレイヤーサイズ補正
#define PLAYER_MAP_COLLISION_OFFSET (0.05f)
// 矩形判定の位置補正
#define PLAYER_BOX_COLLISION_OFFSET_X (24)
#define PLAYER_BOX_COLLISION_OFFSET_Y (20)
// 矩形判定のサイズ
#define PLAYER_BOX_COLLISION_WIDTH (60)
#define PLAYER_BOX_COLLISION_HEIGHT (60)

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
    g_PlayerData.isGround = false;
    g_PlayerData.state = NORMAL;
    g_PlayerData.maxHp = 5;
    g_PlayerData.hp = g_PlayerData.maxHp;
    memset(&g_PlayerData.boxCollision, 0, sizeof(g_PlayerData.boxCollision));
}

void LoadPlayer()
{
    g_PlayerData.handle = LoadGraph("Data/Player/Player.png");
}

void StartPlayer()
{
    g_PlayerData.active = true;
    g_PlayerData.boxCollision.width = PLAYER_BOX_COLLISION_WIDTH;
    g_PlayerData.boxCollision.height = PLAYER_BOX_COLLISION_HEIGHT;
}

void StepPlayer()
{
    g_PlayerData.moveX = 0.0f;

    // 左右入力
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

    // ジャンプ
    if (g_PlayerData.isGround && IsTriggerKey(KEY_Z))
    {
        g_PlayerData.moveY = -PLAYER_JUMP_POWER;
        g_PlayerData.isAir = true;
        g_PlayerData.isGround = false;
    }

    // 重力は空中のみ
    if (!g_PlayerData.isGround)
        g_PlayerData.moveY += PLAYER_GRAVITY;
}

void UpdatePlayer()
{
    if (!g_PlayerData.active) return;

    switch (g_PlayerData.state)
    {
    case NORMAL:
        UpdateNormal(g_PlayerData);
        break;
    case PUSH:
        UpdateGrab(g_PlayerData);
        break;
    case GRAB:
        UpdateGrab(g_PlayerData);
        break;
    case THROW:
        UpdateThrow(g_PlayerData);
        break;
    case DAMAGE:
        UpdateDamage(g_PlayerData);
        break;
    case DEAD:
        UpdateDead(g_PlayerData);
        break;
    }
}

void UpdateNormal(PlayerData& player)
{
    // 位置更新
    player.posX += player.moveX;
    player.posY += player.moveY;

    if (IsInputKey(KEY_C))
    {
        g_PlayerData.state = PUSH;
    }
}

void UpdatePush(PlayerData& player)
{
    player.posX += player.moveX / 2;
}
void UpdateGrab(PlayerData& player)
{
    player.posX += player.moveX; player.posY += player.moveY;
}
void UpdateThrow(PlayerData& player)
{
    player.posX += player.moveX; player.posY += player.moveY;
}

void UpdateDamage(PlayerData& player)
{
    player.posX += player.moveX;
    player.posY += player.moveY;

    static int damageTimer = 0;
    if (damageTimer == 0) damageTimer = 30;
    damageTimer--;
    if (damageTimer <= 0) { damageTimer = 0; player.state = NORMAL; }
}

void UpdateDead(PlayerData& player) { player.moveX = 0; player.moveY = 0; }

extern Camera camera;
void DrawPlayer()
{
    PlayerData* player = GetPlayer();
    if (!player || !player->active) return;

    DrawGraph(
        (int)(player->posX - camera.GetX()),
        (int)(player->posY - camera.GetY()),
        player->handle,
        TRUE
    );
}

void FinPlayer() { DeleteGraph(g_PlayerData.handle); }
PlayerData* GetPlayer() { return &g_PlayerData; }