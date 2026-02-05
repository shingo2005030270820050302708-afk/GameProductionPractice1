#include "DxLib.h"
#include "TitleScene.h"
#include <math.h>
#include "SceneManager.h"
#include "../Input/Input.h"

// タイトルの位置
#define TITLE_POS_X (900)
#define TITLE_POS_Y (200)

// PressZKeyの位置
#define PRESS_Z_KEY_POS_X (900)
#define PRESS_Z_KEY_POS_Y (300)

// PressZKeyの点滅スピード
#define PRESS_Z_KEY_BLINK_SPEED (0.04f)

#define TITLE_GRAVITY   0.15f
#define TITLE_MAX_SPEED 3.0f

// PressZKye点滅用変数
float g_PressZKeyAlpha = 0.0f;	// アルファ値
float g_PressZKeyRadian = 0.0f;	// cos関数に渡すラジアン値

// タイトル演出用
float g_TitlePosY = 0.0f;
float g_TitleSpeed = 0.0f;
int   g_TitleArrived = 0;

void InitTitleScene()
{
	g_PressZKeyAlpha = 0.0f;
	g_PressZKeyRadian = 0.0f;

	g_TitlePosY = -200.0f;
	g_TitleSpeed = 0.0f;
	g_TitleArrived = 0;
}

void LoadTitleScene()
{
}

void StartTitleScene()
{
}

void StepTitleScene()
{
	// ===== タイトル落下演出 =====
	if (!g_TitleArrived)
	{
		g_TitleSpeed += TITLE_GRAVITY;

		if (g_TitleSpeed > TITLE_MAX_SPEED)
			g_TitleSpeed = TITLE_MAX_SPEED;

		g_TitlePosY += g_TitleSpeed;

		if (g_TitlePosY >= TITLE_POS_Y)
		{
			g_TitlePosY = TITLE_POS_Y;
			g_TitleSpeed = 0.0f;
			g_TitleArrived = 1;
		}
	}
	else
	{
		// タイトル到着後にPressZKey点滅
		g_PressZKeyAlpha = (sinf(g_PressZKeyRadian) + 1) / 2;
		g_PressZKeyRadian += PRESS_Z_KEY_BLINK_SPEED;

		if (g_PressZKeyRadian > DX_TWO_PI_F)
			g_PressZKeyRadian -= DX_TWO_PI_F;
	}

	// Zキー入力（タイトル到着後のみ）
	if (g_TitleArrived && IsTriggerKey(KEY_Z))
	{
		ChangeScene(SCENE_PLAY);
	}
	// Xキー入力（タイトル到着後のみ）
	if (g_TitleArrived && IsTriggerKey(KEY_X))
	{
		ChangeScene(SCENE_OPTION);
	}
}

void UpdateTitleScene()
{
}

void DrawTitleScene()
{
	const char* titleText = "課題Git";
	int titleWidth = GetDrawStringWidth(titleText, strlen(titleText));
	int titleX = (640 - titleWidth) / 2;

	DrawString(titleX, (int)g_TitlePosY, titleText, GetColor(255, 255, 255));

	if (g_TitleArrived)
	{
		const char* pressText = "Press Z Button";
		int pressWidth = GetDrawStringWidth(pressText, strlen(pressText));
		int pressX = (640 - pressWidth) / 2;

		int alpha = (int)(g_PressZKeyAlpha * 255);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawString(pressX, PRESS_Z_KEY_POS_Y, pressText, GetColor(255, 255, 255));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void FinTitleScene()
{
	// フォント解放は不要になったので何もしない
}
