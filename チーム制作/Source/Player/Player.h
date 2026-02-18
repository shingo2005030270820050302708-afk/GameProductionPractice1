#pragma once
#include "../Map/MapParameter.h"
#include "../Collision/Collision.h"

#define PLAYER_WIDTH	(64.0f)
#define PLAYER_HEIGHT	(64.0f)
#define PLAYER_RADIUS	(32.0f)

enum PlayerState
{
	NORMAL,
	PUSH,
	GRAB,
	THROW,
	DAMAGE,
	DEAD
};

struct PlayerData
{
	bool active;								// 生存フラグ
	bool isTurn;								// 左を向いているか
	bool isAir;									// 空中にいるか
	float posX;									// X座標
	float posY;									// Y座標
	float moveX;								// X移動量
	float moveY;								// Y移動量
	int hp;
	int maxHp;
	int handle;

	BoxCollision boxCollision;

	PlayerState state;
};

void InitPlayer();
void LoadPlayer();
void StartPlayer();
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();
void UpdateNormal(PlayerData& player);
void UpdateGrab(PlayerData& player);
void UpdatePush(PlayerData& player);
void UpdateThrow(PlayerData& player);
void UpdateDamage(PlayerData& player);
void UpdateDead(PlayerData& player);
PlayerData* GetPlayer();


void PlayerHitNormalBlockX(MapChipData mapChipData);
void PlayerHitNormalBlockY(MapChipData mapChipData);
void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);
