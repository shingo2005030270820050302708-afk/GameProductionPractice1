#pragma once
#include "../Player/Player.h"
#include "../Map/MapParameter.h"
#include "../Collision/Collision.h"
#include "../Block/Block.h"
#define ENEMY_MAX 10
enum NormalEnemyState
{
	Idle,
	Move,
	Attack,
	Dead
};

struct NormalEnemyData
{
	bool active;
	bool isTurn;
	bool isGround;
	bool isAir;
	bool gravity = false;
	VECTOR pos = VGet(0, 0, 0);
	VECTOR vel = VGet(0, 0, 0);
	float moveX;
	float moveY;
	int hp;
	int maxHp;
	int deathTimer = 0;

	int handle = -1;
	BoxCollision boxCollision;

	NormalEnemyState state;
};

extern NormalEnemyData g_NormalEnemyData[ENEMY_MAX];

void InitNormalEnemy();
void LoadNormalEnemy();
void StartNormalEnemy();
void StepNormalEnemy(const PlayerData& player);
void UpdateNormalEnemy(NormalEnemyData& e, const PlayerData& player);
void DrawNormalEnemy();
void FinNormalEnemy();
void UpdateIdle(NormalEnemyData& e, const PlayerData& player);
void UpdateMove(NormalEnemyData& e, const PlayerData& player);
void UpdateAttack(NormalEnemyData& e, const PlayerData& player);
void UpdateDead(NormalEnemyData& e, const PlayerData& player);
void CheckEnemyMapCollision(NormalEnemyData& e);
NormalEnemyData* GetNormalEnemyData();