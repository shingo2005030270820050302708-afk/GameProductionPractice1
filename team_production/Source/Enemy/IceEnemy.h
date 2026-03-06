#pragma once
#include "../Player/Player.h"
#include "../Map/MapParameter.h"
#include "../Collision/Collision.h"
#include "../Block/Block.h"
#define ENEMY_MAX 10
enum IceEnemyState
{
	Idle,
	Move,
	Attack,
	Dead
};

struct IceEnemyData
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

	IceEnemyState state;
};

extern IceEnemyData g_IceEnemyData[ENEMY_MAX];

void InitIceEnemy();
void LoadIceEnemy();
void StartIceEnemy();
void StepIceEnemy(const PlayerData& player);
void UpdateIceEnemy(IceEnemyData& e, const PlayerData& player);
void DrawIceEnemy();
void FinIceEnemy();
void UpdateIdle(IceEnemyData& e, const PlayerData& player);
void UpdateMove(IceEnemyData& e, const PlayerData& player);
void UpdateAttack(IceEnemyData& e, const PlayerData& player);
void UpdateDead(IceEnemyData& e, const PlayerData& player);
void CheckEnemyMapCollision(IceEnemyData& e);
IceEnemyData* GetIceEnemyData();