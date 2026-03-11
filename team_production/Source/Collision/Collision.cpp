#include "DxLib.h"
#include "Collision.h"
#include "../Player/Player.h"
#include "../Enemy/NormalEnemy.h"
#include "../Map/MapBlock.h"
#include "../Map/MapManager.h"
#include <math.h>
#include "../Block/BlockManager.h"

// 矩形と点の当たり判定
bool CheckSquarePoint(float squarePosX, float squarePosY, float squareWidth, float squareHeight, float pointX, float pointY)
{
    if (pointX >= squarePosX && pointX <= (squarePosX + squareWidth))
    {
        if (pointY >= squarePosY && (pointY <= squarePosY + squareHeight))
        {
            return true;
        }
    }

    return false;
}

// 矩形と矩形の当たり判定
bool CheckSquareSquare(float squareA_PosX, float squareA_PosY, float squareA_Width, float squareA_Height, float squareB_PosX, float squareB_PosY, float squareB_Width, float squareB_Height)
{
    float leftA = squareA_PosX;
    float rightA = squareA_PosX + squareA_Width;
    float topA = squareA_PosY;
    float bottomA = squareA_PosY + squareA_Height;

    float leftB = squareB_PosX;
    float rightB = squareB_PosX + squareB_Width;
    float topB = squareB_PosY;
    float bottomB = squareB_PosY + squareB_Height;

    if (rightA >= leftB && leftA <= rightB)
    {
        if (bottomA >= topB && topA <= bottomB)
        {
            return true;
        }
    }

    return false;
}

// 円と点の当たり判定
bool CheckCirclePoint(float circlePosX, float circlePosY, float circleRadius, float pointX, float pointY)
{
    float distance = sqrtf((pointX - circlePosX) * (pointX - circlePosX) + (pointY - circlePosY) * (pointY - circlePosY));

    if (distance <= circleRadius)
    {
        return true;
    }

    return false;
}

// 円と円の当たり判定
bool CheckCircleCircle(float circleA_PosX, float circleA_PosY, float circleA_Radius, float circleB_PosX, float circleB_PosY, float circleB_Radius)
{
    float distance = sqrtf((circleB_PosX - circleA_PosX) * (circleB_PosX - circleA_PosX) + (circleB_PosY - circleA_PosY) * (circleB_PosY - circleA_PosY));

    if (distance <= (circleA_Radius + circleB_Radius))
    {
        return true;
    }

    return false;
}

void CheckCollision()
{
    // プレイヤーとマップの当たり判定
    //CheckMapPlayerCollision();
}

void ResolvePlayerVsDynamicBlock(PlayerData* p, BlockData* b)
{
    // 前提：AABB重なりがある状態で呼ばれる
    float px = p->posX + PLAYER_BOX_COLLISION_OFFSET_X;
    float py = p->posY + PLAYER_BOX_COLLISION_OFFSET_Y;
    float pw = p->boxCollision.width;
    float ph = p->boxCollision.height;

    float bx = b->pos.x;
    float by = b->pos.y;
    float bw = b->width;
    float bh = b->height;

    // オーバーラップ量
    float overlapLeft = (px + pw) - bx;
    float overlapRight = (bx + bw) - px;
    float overlapTop = (py + ph) - by;
    float overlapBottom = (by + bh) - py;

    float minOverlapX = (overlapLeft < overlapRight ? overlapLeft : overlapRight);
    float minOverlapY = (overlapTop < overlapBottom ? overlapTop : overlapBottom);

    const float EPS = 0.0001f;
    bool blockMovingDown = (b->vel.y > 0.0f) || (b->pos.y - b->prevPos.y > 0.0f);

    // 通常の軸選択
    bool resolveXFirst = (minOverlapX + EPS < minOverlapY);

    if (!resolveXFirst)
    {
        // 縦方向が本来小さい ＝ 通常なら Y で解決
        if (overlapTop + EPS < overlapBottom)
        {
            // 上から着地（プレイヤーがブロック上に立つ）
            p->posY = by - ph;
            p->moveY = 0;
            p->isGround = true;
            p->isAir = false;
        }
        else
        {
            // ここは「下から突き上げ」側
            // もしブロックが落下中なら、Yで押さずにXで解決へ切替
            if (blockMovingDown)
            {
                // Xで解決
                if (overlapLeft < overlapRight)
                    p->posX = bx - pw;
                else
                    p->posX = bx + bw;

                p->moveX = 0;
                // Yは触らない：下方向へ弾かない
            }
            else
            {
                // 通常の下方向解決
                p->posY = by + bh;
                p->moveY = 0;
            }
        }
    }
    else
    {
        // Xで解決
        if (overlapLeft < overlapRight)
            p->posX = bx - pw;
        else
            p->posX = bx + bw;

        p->moveX = 0;
    }
}


void ResolveEnemyBlockCollision(NormalEnemyData& e, BlockData* block)
{
    float ex = e.pos.x;
    float ey = e.pos.y;
    float ew = e.boxCollision.width;
    float eh = e.boxCollision.height;

    float bx = block->pos.x;
    float by = block->pos.y;
    float bw = block->width;
    float bh = block->height;

    // 中心座標
    float ecx = ex + ew * 0.5f;
    float ecy = ey + eh * 0.5f;
    float bcx = bx + bw * 0.5f;
    float bcy = by + bh * 0.5f;

    // 中心間距離
    float dx = ecx - bcx;
    float dy = ecy - bcy;

    // 重なり量
    float overlapX = (ew * 0.5f + bw * 0.5f) - fabsf(dx);
    float overlapY = (eh * 0.5f + bh * 0.5f) - fabsf(dy);

    // 横方向の押し戻し
    if (overlapX < overlapY)
    {
        if (dx > 0)
            e.pos.x += overlapX;   // 右側に押し戻す
        else
            e.pos.x -= overlapX;   // 左側に押し戻す

        e.vel.x = 0;
    }
    // 縦方向の押し戻し
    else
    {
        if (dy > 0)
        {
            e.pos.y += overlapY;   // 下から押し上げる
            e.vel.y = 0;
        }
        else
        {
            e.pos.y -= overlapY;   // 上から落ちてきた
            e.vel.y = 0;
            e.isGround = true;
            e.isAir = false;
        }
    }
}

void CheckPlayerMapCollision()
{
    PlayerData* p = GetPlayer();
    if (!p) return;

    float px = p->posX + PLAYER_BOX_COLLISION_OFFSET_X;
    float py = p->posY + PLAYER_BOX_COLLISION_OFFSET_Y;
    float pw = p->boxCollision.width;
    float ph = p->boxCollision.height;

    int leftTile = (int)(px / MAP_CHIP_WIDTH);
    int rightTile = (int)((px + pw) / MAP_CHIP_WIDTH);
    int topTile = (int)(py / MAP_CHIP_HEIGHT);
    int bottomTile = (int)((py + ph) / MAP_CHIP_HEIGHT);

    // 毎フレーム地上判定初期化
    p->isGround = false;

    for (int y = topTile; y <= bottomTile; y++)
    {
        for (int x = leftTile; x <= rightTile; x++)
        {
            MapChipData chip = GetMapChipData(x, y);
            if (chip.mapChip == MAP_CHIP_NONE) continue;

            BlockData* block = chip.data;
            if (!block || !block->active) continue;

            // COIN_BLOCK は衝突しない
            if (chip.mapChip == COIN_BLOCK) continue;

            // ゴール判定
            if (chip.mapChip == GOAL_BLOCK)
            {
                if (CheckSquareSquare(px, py, pw, ph,
                    block->pos.x, block->pos.y, block->width, block->height))
                {
                    p->isGoal = true;
                    return;
                }
            }

            // 通常ブロックの衝突判定
            if (CheckSquareSquare(
                px, py, pw, ph,
                block->pos.x, block->pos.y, block->width, block->height))
            {
                ResolvePlayerVsDynamicBlock(p, block);
            }
        }
    }
}

void CheckEnemyMapCollision(NormalEnemyData& e)
{
    float ex = e.pos.x;
    float ey = e.pos.y;
    float ew = e.boxCollision.width;
    float eh = e.boxCollision.height;

    int leftTile = (int)(ex / MAP_CHIP_WIDTH);
    int rightTile = (int)((ex + ew) / MAP_CHIP_WIDTH);
    int topTile = (int)(ey / MAP_CHIP_HEIGHT);
    int bottomTile = (int)((ey + eh) / MAP_CHIP_HEIGHT);

    e.isGround = false;

    for (int y = topTile; y <= bottomTile; y++)
    {
        for (int x = leftTile; x <= rightTile; x++)
        {
            MapChipData chip = GetMapChipData(x, y);
            if (chip.mapChip == MAP_CHIP_NONE) continue;

            BlockData* block = chip.data;
            if (!block || !block->active) continue;

            // 敵とブロックの当たり判定
            if (CheckSquareSquare(
                ex, ey, ew, eh,
                block->pos.x, block->pos.y, block->width, block->height))
            {
                ResolveEnemyBlockCollision(e, block);
            }
        }
    }
}


// 動くブロック同士の衝突
void ResolveBlockCollision(BlockData& a, BlockData& b)
{
    if (!a.active || !b.active) return;

    float aLeft = a.pos.x;
    float aRight = a.pos.x + a.width;
    float aTop = a.pos.y;
    float aBottom = a.pos.y + a.height;

    float bLeft = b.pos.x;
    float bRight = b.pos.x + b.width;
    float bTop = b.pos.y;
    float bBottom = b.pos.y + b.height;

    float overlapX = (aRight - bLeft < bRight - aLeft) ? (aRight - bLeft) : -(bRight - aLeft);
    float overlapY = (aBottom - bTop < bBottom - aTop) ? (aBottom - bTop) : -(bBottom - aTop);

    const float friction = 0.5f; // 摩擦係数

    if (fabs(overlapX) < fabs(overlapY))
    {
        // 横方向の押し戻し
        a.pos.x -= overlapX / 2.0f;
        b.pos.x += overlapX / 2.0f;
        a.vel.x = 0;
        b.vel.x = 0;
    }
    else
    {
        // 縦方向の押し戻し
        a.pos.y -= overlapY / 1.0f;
        b.pos.y += overlapY / 1.0f;
        a.vel.y = 0;
        b.vel.y = 0;

        // 縦押し戻しのときに横速度に摩擦をかける
        a.vel.x *= friction;
        b.vel.x *= friction;
    }
}


void ResolveBlockMapCollision(BlockData& movingBlock, BlockData& mapBlock)
{
    if (!movingBlock.active || !mapBlock.active) return;

    float aLeft = movingBlock.pos.x;
    float aRight = movingBlock.pos.x + movingBlock.width;
    float aTop = movingBlock.pos.y;
    float aBottom = movingBlock.pos.y + movingBlock.height;

    float bLeft = mapBlock.pos.x;
    float bRight = mapBlock.pos.x + mapBlock.width;
    float bTop = mapBlock.pos.y;
    float bBottom = mapBlock.pos.y + mapBlock.height;

    float overlapX = (aRight - bLeft < bRight - aLeft) ? (aRight - bLeft) : -(bRight - aLeft);
    float overlapY = (aBottom - bTop < bBottom - aTop) ? (aBottom - bTop) : -(bBottom - aTop);

    const float friction = 0.5f; // 摩擦係数

    if (fabs(overlapX) < fabs(overlapY))
    {
        // 横方向の押し戻し
        movingBlock.pos.x -= overlapX;
        movingBlock.vel.x = 0;
    }
    else
    {
        // 縦方向の押し戻し
        movingBlock.pos.y -= overlapY;
        movingBlock.vel.y = 0;
        movingBlock.vel.x *= friction; // 横速度に摩擦をかける
    }
}

void ResolveEnemyCollision(NormalEnemyData& a, NormalEnemyData& b)
{
    if (!a.active || !b.active) return;

    float aLeft = a.pos.x;
    float aRight = a.pos.x + a.boxCollision.width;
    float aTop = a.pos.y;
    float aBottom = a.pos.y + a.boxCollision.height;

    float bLeft = b.pos.x;
    float bRight = b.pos.x + b.boxCollision.width;
    float bTop = b.pos.y;
    float bBottom = b.pos.y + b.boxCollision.height;

    // 重なり量
    float overlapX = (aRight - bLeft < bRight - aLeft)
        ? (aRight - bLeft)
        : -(bRight - aLeft);

    float overlapY = (aBottom - bTop < bBottom - aTop)
        ? (aBottom - bTop)
        : -(bBottom - aTop);

    const float friction = 0.5f;

    // 横方向の押し戻し
    if (fabs(overlapX) < fabs(overlapY))
    {
        a.pos.x -= overlapX / 2.0f;
        b.pos.x += overlapX / 2.0f;

        a.vel.x = 0;
        b.vel.x = 0;
    }
    // 縦方向の押し戻し
    else
    {
        a.pos.y -= overlapY / 2.0f;
        b.pos.y += overlapY / 2.0f;

        a.vel.y = 0;
        b.vel.y = 0;

        // 摩擦
        a.vel.x *= friction;
        b.vel.x *= friction;
    }
}