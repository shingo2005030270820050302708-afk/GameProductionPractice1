#include "DxLib.h"
#include "Animation.h"

// 初期化
void InitAnimation(AnimationData* animData)
{
	animData->active = false;
	animData->isLoop = false;
	animData->handle = 0;
	animData->nowFrame = 0;
	animData->frameNum = 0;
	animData->frameWidth = 0;
	animData->frameHeight = 0;
	animData->timer = 0;
	animData->interval = 0;
	animData->posX = 0.0f;
	animData->posY = 0.0f;
	animData->frameIndex = 0;
	animData->frameWait = 0;
}


// 再生開始
void StartAnimation(AnimationData* animData, float posX, float posY, int interval, int frameNum, int width, int height, bool isLoop)
{
	animData->active = true;
	animData->posX = posX;
	animData->posY = posY;

	animData->interval = interval;
	animData->timer = interval;

	animData->nowFrame = 0;
	animData->frameNum = frameNum;

	animData->frameWidth = width;
	animData->frameHeight = height;

	animData->isLoop = isLoop;
	animData->frameIndex = 0;

	animData->frameWait = 0;
}


// 更新
void UpdateAnimation(AnimationData* animData)
{
	if (!animData->active) return;

	// 待ち時間
	if (animData->frameWait > 0)
	{
		animData->frameWait--;
		return;
	}

	animData->timer--;

	if (animData->timer <= 0)
	{
		animData->nowFrame++;
		animData->timer = animData->interval;

		if (animData->nowFrame >= animData->frameNum)
		{
			if (animData->isLoop)
			{
				animData->nowFrame = 0;
			}
			else
			{
				animData->nowFrame = animData->frameNum - 1;
				animData->active = false;
			}
		}
	}

	animData->frameIndex = animData->nowFrame;
}


// 描画（内部座標）
void DrawAnimation(AnimationData* animData, bool isTurnX, bool isTurnY)
{
	DrawAnimation(animData, animData->posX, animData->posY, isTurnX, isTurnY);
}


// 描画（位置指定付き）
void DrawAnimation(AnimationData* animData, float posX, float posY, bool isTurnX, bool isTurnY)
{
	if (!animData->active) return;
	if (animData->handle <= 0) return;

	int frameX = animData->frameWidth * animData->nowFrame;

	DrawRectGraph(
		(int)posX,
		(int)posY,
		frameX,
		0,
		animData->frameWidth,
		animData->frameHeight,
		animData->handle,
		TRUE,
		isTurnX,
		isTurnY
	);
}