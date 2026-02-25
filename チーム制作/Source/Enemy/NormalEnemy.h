#pragma once
#include "../Map/MapParameter.h"
#include "../Collision/Collision.h"


enum NormalEnemyState
{
    Idle,
    Move,
    Attack,
    Damage,
    Dead
};

struct NormalEnemyData
{
	bool active;								// 生存フラグ
	bool isTurn;								// 左を向いているか
	bool isGround;
	bool isAir;									// 空中にいるか
	float posX;									// X座標
	float posY;									// Y座標
	float moveX;								// X移動量
	float moveY;								// Y移動量
	int hp;
	int maxHp;
	int handle;

	BoxCollision boxCollision;

	NormalEnemyState state;
};