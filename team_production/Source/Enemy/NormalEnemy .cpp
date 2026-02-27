#include "DxLib.h"
#include "NormalEnemy.h"
#include "../Player/Player.h"
#include "../Collision/Collision.h"
#include "../../Data/Camera/Camera.h"
#include "../Map/MapChip.h"
#include "../Block/Block.h"

NormalEnemyData g_NormalEnemyData[ENEMY_MAX];

void InitNormalEnemy()
{
}
void LoadNormalEnemy()
{
}
void StartNormalEnemy()
{
}
void StepNormalEnemy()
{
    for (int i = 0; i < ENEMY_MAX; i++)
    {
        NormalEnemyData& e = g_NormalEnemyData[i];
        if (!e.active) continue;

        // Enemy の当たり判定
        float ex = e.posX;
        float ey = e.posY;
        float ew = e.boxCollision.width;
        float eh = e.boxCollision.height;

        // --- Block との衝突チェック ---
        for (int j = 0; j < BLOCK_MAX; j++)
        {
            BlockData& b = g_Block[j];
            if (!b.active) continue;

            if (b.state != BLOCK_THROW)
                continue;

            float bx = b.pos.x;
            float by = b.pos.y;
            float bw = b.width;
            float bh = b.height;

            if (CheckSquareSquare(ex, ey, ew, eh, bx, by, bw, bh))
            {
                e.state = Dead;
                e.active = false;
                break;  // この敵の処理は終了
            }
        }
    }
}

void UpdaterNormalEnemy()
{
}
void DrawNormalEnemy()
{
}
void FinNormalEnemy()
{
}
void UpdateDamage(NormalEnemyData& enemy)
{
}
void UpdateDead(NormalEnemyData& enemy)
{
}

    // --- Enemy の通常処理 ---
    // 移動・重力・アニメーションなど