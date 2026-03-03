#include "DxLib.h"
#include "Collision.h"
#include "../Player/Player.h"
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

void ResolvePlayerBlockCollision(BlockData* block)
{
    PlayerData* p = GetPlayer();
    if (!p) return;

    float px = p->posX;
    float py = p->posY;
    float pw = p->boxCollision.width;
    float ph = p->boxCollision.height;

    float bx = block->pos.x;
    float by = block->pos.y;
    float bw = block->width;
    float bh = block->height;

    // どちらの方向にめり込んでいるか判定
    float overlapLeft = (px + pw) - bx;
    float overlapRight = (bx + bw) - px;
    float overlapTop = (py + ph) - by;
    float overlapBottom = (by + bh) - py;

    float minOverlapX = (overlapLeft < overlapRight ? overlapLeft : overlapRight);
    float minOverlapY = (overlapTop < overlapBottom ? overlapTop : overlapBottom);

    // 横方向の押し戻しが小さい
    if (minOverlapX < minOverlapY)
    {
        if (overlapLeft < overlapRight)
            p->posX = bx - pw;       // 左からぶつかった
        else
            p->posX = bx + bw;       // 右からぶつかった

        p->moveX = 0;
    }
    // 縦方向の衝突
    else
    {
        if (overlapTop < overlapBottom)
        {
            p->posY = by - ph;       // 上から落ちてきた
            p->moveY = 0;
            p->isGround = true;
            p->isAir = false;
        }
        else
        {
            p->posY = by + bh;       // 下から突き上げた
            p->moveY = 0;
        }
    }
}
void CheckPlayerMapCollision()
{
    PlayerData* p = GetPlayer();
    if (!p) return;

    float px = p->posX;
    float py = p->posY;
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

            
            if (chip.mapChip == GOAL_BLOCK)
            {
                // プレイヤーがゴールに触れた！
                if (CheckSquareSquare(px, py, pw, ph,
                    block->pos.x, block->pos.y, block->width, block->height))
                {
                    // クリアフラグ ON
                    p->isGoal = true;
                    return; // これ以上判定しなくてもいい
                }
            }
        
            // 通常の衝突判定
            if (CheckSquareSquare(
                px, py, pw, ph,
                block->pos.x, block->pos.y, block->width, block->height))
            {
                ResolvePlayerBlockCollision(block);
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

// 動くブロック同士の衝突判定
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