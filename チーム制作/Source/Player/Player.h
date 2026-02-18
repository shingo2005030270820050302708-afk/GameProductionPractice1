#pragma once
#include "../Map/MapParameter.h"
#include "../Collision/Collision.h"

#define PLAYER_WIDTH	(64.0f)
#define PLAYER_HEIGHT	(64.0f)
#define PLAYER_RADIUS	(32.0f)

struct PlayerData
{
	bool active;								// 生存フラグ
	bool isTurn;								// 左を向いているか
	bool isAir;									// 空中にいるか
	float posX;									// X座標
	float posY;									// Y座標
	float moveX;								// X移動量
	float moveY;								// Y移動量
	int handle;
	BoxCollision boxCollision;
};

void InitPlayer();
void LoadPlayer();
void StartPlayer();
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();

PlayerData* GetPlayer();


void PlayerHitNormalBlockX(MapChipData mapChipData);
void PlayerHitNormalBlockY(MapChipData mapChipData);
void CalcBoxCollision(PlayerData player, float& x, float& y, float& w, float& h);
