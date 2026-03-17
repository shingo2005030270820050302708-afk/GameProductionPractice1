#include "DxLib.h"
#include "Player.h"
#include "../Input/Input.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"
#include "../Map/MapChip.h"
#include "../Enemy/NormalEnemy.h"
#include "../Scene/GlobalData.h"


//Player初期スポーン
#define PLAYER_DEFAULT_POS_X (100.0f)
#define PLAYER_DEFAULT_POS_Y (640.0f)

// Playerの諸々のステータス
#define PLAYER_MOVE_SPEED (2.5f)
#define PLAYER_JUMP_POWER (11.5f)
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
    {10, 4, 40, 64},//IDLE
    {6, 6, 40, 64},//RUN
    {10, 8, 40, 64},//JUMP
    {4, 6, 64, 64},//PUSH
    {4, 4, 64, 64},//CLIM
    {4, 8, 64, 64},//DEATH
    {4, 4, 64, 64},//HURT
    {6, 4, 40, 64},//THROW
    {10, 6, 40, 64},//DUST1(RUN,PUSH)
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
    g_PlayerData.maxHp = 1;
    g_PlayerData.isGoal = false;
    g_PlayerData.hp = g_PlayerData.maxHp;
    g_PlayerData.invincibleTimer = 0;
    g_PlayerData.deadTimer = 0;
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

    if (g_PlayerData.state == NORMAL)
    {
        StepPlayer();
    }

    if (g_PlayerData.posY > 900.0f && g_PlayerData.state != DEAD)
    {
        g_PlayerData.state = DEAD;
        g_PlayerData.deadTimer = 0;  
      
        return; 
    }

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
        for (int i = 0; i < ENEMY_MAX; i++)
        {
            CheckPlayerEnemyCollision(g_NormalEnemyData[i], g_PlayerData);
        }
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

    player.hp--;

    // HP が 0 なら死亡
    if (player.hp <= 0)
    {
        player.state = DEAD;
    }

    player.invincibleTimer--;

    if (player.invincibleTimer <= 0)
    {
        player.invincibleTimer = 0;

        player.state = NORMAL;
    }

}

void UpdateDead(PlayerData& player)
{
    static bool counted = false;

    if (!counted)
    {
        gData.deathCount++;
        counted = true;
    }

    if (player.deadTimer == 0)
    {
        player.deadTimer = 30;   // 30フレーム待つ
    }

    player.deadTimer--;

    // タイマーが終わったらリスポーン
    if (player.deadTimer <= 0)
    {
        player.deadTimer = 0;

        player.moveX = 0;
        player.moveY = 0;

        player.hp = player.maxHp;
        player.posX = PLAYER_DEFAULT_POS_X;
        player.posY = PLAYER_DEFAULT_POS_Y;

        player.state = NORMAL;



        counted = false; // リスポーン時に再度カウントできるようにする

        camera.SetPosition(PLAYER_DEFAULT_POS_X, PLAYER_DEFAULT_POS_Y);
    
    }
}
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
        // 念のため範囲チェックw
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

#if 1 // デバッグ用

    float px = p->posX + PLAYER_BOX_COLLISION_OFFSET_X - camera.GetX();
    float py = p->posY + PLAYER_BOX_COLLISION_OFFSET_Y - camera.GetY();
    float pw = p->boxCollision.width;
    float ph = p->boxCollision.height;

    DrawBox(
        (int)px,
        (int)py,
        (int)(px + pw),
        (int)(py + ph),
        GetColor(255, 0, 0),
        FALSE
    );

#endif
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

void CheckPlayerEnemyCollision(NormalEnemyData& e, PlayerData& p)
{
    // 敵が非アクティブなら何もしない
    if (!e.active) return;

    // プレイヤーの当たり判定
    float px = p.posX;
    float py = p.posY;
    float pw = p.boxCollision.width;
    float ph = p.boxCollision.height;

    // 敵の当たり判定
    float ex = e.pos.x;
    float ey = e.pos.y;
    float ew = e.boxCollision.width;
    float eh = e.boxCollision.height;

    // AABB衝突判定
    bool hit =
        px < ex + ew &&
        px + pw > ex &&
        py < ey + eh &&
        py + ph > ey;

    if (hit)
    {
        // ダメージは NORMAL のときだけ
        if (p.state == NORMAL)
        {
            p.state = DAMAGE;

            p.invincibleTimer = 90;
            // ノックバック方向（敵の位置から計算）
            p.moveX = (p.posX < ex ? -3.0f : 3.0f);
            p.moveY = -5.0f;
        }
    }
}