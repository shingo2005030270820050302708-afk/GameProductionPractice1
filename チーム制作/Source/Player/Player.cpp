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
#define PLAYER_MOVE_SPEED (3.0f)
#define PLAYER_JUMP_POWER (7.0f)
#define PLAYER_GRAVITY (0.4f)

// マップ衝突判定用のプレイヤーサイズ補正
#define PLAYER_MAP_COLLISION_OFFSET (0.05f)

PlayerData g_PlayerData = { 0 };
PlayerData g_PrevPlayerData = { 0 };

struct PlayerAnimationParam
{
    int interval;
    int frameNum;
    int width;
    int height;
};

const PlayerAnimationParam PLAYER_ANIM_PARAM[PLAYER_ANIM_MAX] =
{
    {10, 4, 64, 64},//IDLE
    {6, 6, 64, 64},//RUN
    {10, 8, 64, 64},//JUMP
    {4, 6, 64, 64},//PUSH
    {4, 4, 64, 64},//CLIM
    {4, 8, 64, 64},//DEATH
    {4, 4, 64, 64},//HURT
    {6, 4, 64, 64},//THROW
    {6, 6, 64, 64},//DUST1(RUN,PUSH)
    {6, 5, 64, 64},//DUST2(JUMP)
  
};

void InitPlayer()
{
    g_PlayerData.posX = PLAYER_DEFAULT_POS_X;
    g_PlayerData.posY = PLAYER_DEFAULT_POS_Y;
    g_PlayerData.moveX = 0.0f;
    g_PlayerData.moveY = 0.0f;
    g_PlayerData.active = false;
    g_PlayerData.isAir = false;
    g_PlayerData.isGround = false;
    g_PlayerData.playAnim = PLAYER_ANIM_NONE;
    g_PlayerData.state = NORMAL;
    g_PlayerData.maxHp = 5;
    g_PlayerData.hp = g_PlayerData.maxHp;
    for (int i = 0; i < PLAYER_ANIM_MAX; i++)
        InitAnimation(&g_PlayerData.animation[i]);
    memset(&g_PlayerData.boxCollision, 0, sizeof(g_PlayerData.boxCollision));
}

void LoadPlayer()
{

    g_PlayerData.animation[PLAYER_ANIM_IDLE].handle = LoadGraph("Data/Player/Player_Idle.png");
    g_PlayerData.animation[PLAYER_ANIM_RUN].handle = LoadGraph("Data/Player/Player_Run.png");
    g_PlayerData.animation[PLAYER_ANIM_JUMP].handle = LoadGraph("Data/Player/Player_Jump.png");
    g_PlayerData.animation[PLAYER_ANIM_PUSH].handle = LoadGraph("Data/Player/Anim/push.png");
    g_PlayerData.animation[PLAYER_ANIM_CLIM].handle = LoadGraph("Data/Player/Anim/clim.png");
    g_PlayerData.animation[PLAYER_ANIM_DEATH].handle = LoadGraph("Data/Player/Player_Death.png");
    g_PlayerData.animation[PLAYER_ANIM_HURT].handle = LoadGraph("Data/Player/Player_Hurt.png");
    g_PlayerData.animation[PLAYER_ANIM_THROW].handle = LoadGraph("Data/Player/Player_Throw.png");
    g_PlayerData.animation[PLAYER_ANIM_DUST1].handle = LoadGraph("Data/Player/Player_Dust1.png");
    g_PlayerData.animation[PLAYER_ANIM_DUST2].handle = LoadGraph("Data/Player/Player_Dust2.png");
}
void StartPlayer()
{
    g_PlayerData.active = true;
    StartPlayerAnimation(PLAYER_ANIM_IDLE);
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

    if (g_PlayerData.isThrowing)
    {
        g_PlayerData.playAnim = PLAYER_ANIM_THROW;

        UpdateAnimation(&g_PlayerData.animation[PLAYER_ANIM_THROW]);

        g_PlayerData.throwAnimTimer--;
        if (g_PlayerData.throwAnimTimer <= 0)
        {
            g_PlayerData.isThrowing = false;
        }
    }

    switch (g_PlayerData.state)
    {
    case NORMAL:
        UpdateNormal(g_PlayerData);
        break;
    case DAMAGE:
        UpdateDamage(g_PlayerData);
        SetPlayerAnimation(PLAYER_ANIM_HURT);
        break;
    case DEAD:
        UpdateDead(g_PlayerData);
        SetPlayerAnimation(PLAYER_ANIM_DEATH);
        break;
    }

    if (!g_PlayerData.isThrowing && g_PlayerData.state == NORMAL)
    {
        if (!g_PlayerData.isGround)
        {
            SetPlayerAnimation(PLAYER_ANIM_JUMP);
        }
        else if (g_PlayerData.moveX != 0)
        {
            SetPlayerAnimation(PLAYER_ANIM_RUN);
        }
        else
        {
            SetPlayerAnimation(PLAYER_ANIM_IDLE);
        }
    }

    UpdatePlayerAnimation();

    CheckPlayerMapCollision();
}
void UpdateNormal(PlayerData& player)
{
    // 位置更新
    player.posX += player.moveX;
    player.posY += player.moveY;
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
    PlayerData* p = GetPlayer();
    if (!p || !p->active) return;

    // 描画位置はカメラ補正
    float drawX = p->posX - camera.GetX();
    float drawY = p->posY - camera.GetY();
    if (p->playAnim != PLAYER_ANIM_NONE)
    {
        DrawAnimation(
            &p->animation[p->playAnim],
            drawX,
            drawY,
            p->isTurn,  // 左右反転
            false       // 上下反転なし
        );
    }
    for (auto anim : p->extraAnims)
    {
        // 念のため範囲チェック
        if (anim >= 0 && anim < PLAYER_ANIM_MAX)
        {
            DrawAnimation(
                &p->animation[anim],
                drawX,
                drawY,
                p->isTurn,
                false
            );
        }
    }
}
void StartPlayerAnimation(PlayerAnimationType anim)
{
    if (anim == g_PlayerData.playAnim) return;
    g_PlayerData.playAnim = anim;
    AnimationData* animData = &g_PlayerData.animation[anim];
    PlayerAnimationParam animParam = PLAYER_ANIM_PARAM[anim];
    StartAnimation(animData, g_PlayerData.posX, g_PlayerData.posY, animParam.interval, animParam.frameNum, animParam.width, animParam.height, true);
}
void SetPlayerAnimation(PlayerAnimationType anim)
{
    if (g_PlayerData.playAnim == anim)
        return; // 同じアニメ中なら変更しない

    g_PlayerData.playAnim = anim;

    const PlayerAnimationParam& p = PLAYER_ANIM_PARAM[anim];

    StartAnimation(
        &g_PlayerData.animation[anim],
        g_PlayerData.posX,
        g_PlayerData.posY,
        p.interval,
        p.frameNum,
        p.width,
        p.height,
        true
    );
}

void UpdatePlayerAnimation() {
    // 毎フレームリセット
    g_PlayerData.extraAnims.clear();
    if (g_PlayerData.isGround && g_PlayerData.moveX != 0)
    {
        g_PlayerData.extraAnims.push_back(PLAYER_ANIM_DUST1);
    }
    if (!g_PlayerData.isGround && g_PlayerData.moveY < 0.0f)
    {
        g_PlayerData.extraAnims.push_back(PLAYER_ANIM_DUST2);
    }

    // 基本アニメの更新
    UpdateAnimation(&g_PlayerData.animation[g_PlayerData.playAnim]);

    // 重ねアニメの更新
    for (auto anim : g_PlayerData.extraAnims)
    {
        AnimationData* a = &g_PlayerData.animation[anim];
        if (!a->active)
        {
            const PlayerAnimationParam& p = PLAYER_ANIM_PARAM[anim];
            StartAnimation(a, g_PlayerData.posX, g_PlayerData.posY,
                p.interval, p.frameNum, p.width, p.height, true);
        }
        UpdateAnimation(a);
    }
}
void FinPlayer() { DeleteGraph(g_PlayerData.handle); }
PlayerData* GetPlayer() { return &g_PlayerData; }