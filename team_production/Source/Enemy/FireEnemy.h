//#pragma once
//#include "../Player/Player.h"
//#include "../Map/MapParameter.h"
//#include "../Collision/Collision.h"
//#include "../Block/Block.h"
//#define ENEMY_MAX 10
//enum FireEnemyState
//{
//	Idle,
//	Move,
//	Attack,
//	Dead
//};
//
//struct FireEnemyData
//{
//	bool active;
//	bool isTurn;
//	bool isGround;
//	bool isAir;
//	bool gravity = false;
//	VECTOR pos = VGet(0, 0, 0);
//	VECTOR vel = VGet(0, 0, 0);
//	float moveX;
//	float moveY;
//	int hp;
//	int maxHp;
//	int deathTimer = 0;
//
//	int handle = -1;
//	BoxCollision boxCollision;
//
//	FireEnemyState state;
//};
//
//extern FireEnemyData g_IceEnemyData[ENEMY_MAX];
//
//void InitFireEnemy();
//void LoadFireEnemy();
//void StartFireEnemy();
//void StepFireEnemy(const PlayerData& player);
//void UpdateFireEnemy(FireEnemyData& e, const PlayerData& player);
//void DrawFireEnemy();
//void FinFireEnemy();
//void UpdateIdle(FireEnemyData& e, const PlayerData& player);
//void UpdateMove(FireEnemyData& e, const PlayerData& player);
//void UpdateAttack(FireEnemyData& e, const PlayerData& player);
//void UpdateDead(FireEnemyData& e, const PlayerData& player);
//void CheckEnemyMapCollision(FireEnemyData& e);
//FireEnemyData* GetFireEnemyData();