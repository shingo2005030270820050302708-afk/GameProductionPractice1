#pragma once
#include "../Player/Player.h"
#include "../Map/MapParameter.h"
#include "../Collision/Collision.h"
#include "../Block/Block.h"
#define ENEMY_MAX 10
enum NormalEnemyState
{
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
	float walkTimer = 0;
	float changeInterval = 60;
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
void UpdateMove(NormalEnemyData& e, const PlayerData& player);
void UpdateAttack(NormalEnemyData& e, const PlayerData& player);
void UpdateDead(NormalEnemyData& e);
void CheckEnemyMapCollision(NormalEnemyData& e);
NormalEnemyData* GetNormalEnemyData();