//#pragma once
//#include "../Player/Player.h"
//#include "../Map/MapParameter.h"
//#include "../Collision/Collision.h"
//#include "../Block/Block.h"
//#define ENEMY_MAX 10
//enum WoodEnemyState
//{
//	Idle,
//	Move,
//	Attack,
//	Dead
//};
//
//struct WoodEnemyData
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
//	WoodEnemyState state;
//};
//
//extern WoodEnemyData g_IceEnemyData[ENEMY_MAX];
//
//void InitWoodEnemy();
//void LoadWoodEnemy();
//void StartWoodEnemy();
//void StepWoodEnemy(const PlayerData& player);
//void UpdateWoodEnemy(WoodEnemyData& e, const PlayerData& player);
//void DrawWoodEnemy();
//void FinWoodEnemy();
//void UpdateIdle(WoodEnemyData& e, const PlayerData& player);
//void UpdateMove(WoodEnemyData& e, const PlayerData& player);
//void UpdateAttack(WoodEnemyData& e, const PlayerData& player);
//void UpdateDead(WoodEnemyData& e, const PlayerData& player);
//void CheckEnemyMapCollision(WoodEnemyData& e);
//WoodEnemyData* GetWoodEnemyData();