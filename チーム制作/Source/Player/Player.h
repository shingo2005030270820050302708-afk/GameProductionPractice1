#pragma once
#include "../Map/MapParameter.h"
#include "../Collision/Collision.h"
#include "../Animation/Animation.h"
#include <vector>

#define PLAYER_WIDTH	(64.0f)
#define PLAYER_HEIGHT	(64.0f)
#define PLAYER_RADIUS	(32.0f)
#define PLAYER_BOX_COLLISION_OFFSET_X (0)
#define PLAYER_BOX_COLLISION_OFFSET_Y (0)
#define PLAYER_BOX_COLLISION_WIDTH (64)
#define PLAYER_BOX_COLLISION_HEIGHT (64)

enum PlayerAnimationType {
	PLAYER_ANIM_IDLE,
	PLAYER_ANIM_RUN,
	PLAYER_ANIM_JUMP,
	PLAYER_ANIM_PUSH,
	PLAYER_ANIM_CLIM,
	PLAYER_ANIM_DEATH,
	PLAYER_ANIM_HURT,
	PLAYER_ANIM_THROW,
	PLAYER_ANIM_DUST1,
	PLAYER_ANIM_DUST2,
	PLAYER_ANIM_MAX,
	PLAYER_ANIM_NONE = -1
};

enum PlayerState
{
	NORMAL,
	DAMAGE,
	DEAD
};

struct PlayerData
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
	bool isThrowing;    
	int throwAnimTimer;


	AnimationData animation[PLAYER_ANIM_MAX];
	PlayerAnimationType playAnim;           // 基本アニメ
	std::vector<PlayerAnimationType> extraAnims; // 重ねて流すアニメ
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
void UpdateDamage(PlayerData& player);
void UpdateDead(PlayerData& player);
void StartPlayerAnimation(PlayerAnimationType anim);
void SetPlayerAnimation(PlayerAnimationType anim);
void UpdatePlayerAnimation();
PlayerData* GetPlayer();
extern PlayerData g_PlayerData;


