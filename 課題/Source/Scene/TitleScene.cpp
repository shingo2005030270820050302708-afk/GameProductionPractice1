#include "DxLib.h"
#include "TitleScene.h"
#include <math.h>
#include "SceneManager.h"

// タイトルの位置
#define TITLE_POS_X (40)
#define TITLE_POS_Y (60)

// PressZKeyの位置
#define PRESS_Z_KEY_POS_X (40)
#define PRESS_Z_KEY_POS_Y (350)

// PressZKeyの点滅スピード
#define PRESS_Z_KEY_BLINK_SPEED (0.04f)

#define TITLE_GRAVITY   0.15f
#define TITLE_MAX_SPEED 3.0f



int g_TitleFont = -1;

// 画像ハンドル
int g_TitleHandle = 0;
int g_PressZKeyHandle = 0;

// PressZKye点滅用変数
float g_PressZKeyAlpha = 0.0f;	// アルファ値
float g_PressZKeyRadian = 0.0f;	// cos関数に渡すラジアン値

// タイトル演出用
float g_TitlePosY = 0.0f;
float g_TitleSpeed = 0.0f;
int   g_TitleArrived = 0;

void InitTitleScene()
{
	g_TitleHandle = 0;
	g_PressZKeyHandle = 0;
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



}


void UpdateTitleScene()
{

}

void DrawTitleScene()
{
	const char* titleText = "課題Git";
	int titleWidth = GetDrawStringWidthToHandle(titleText, strlen(titleText), g_TitleFont);
	int titleX = (640 - titleWidth) / 2;

	DrawStringToHandle(titleX, (int)g_TitlePosY, titleText, GetColor(255, 255, 255), g_TitleFont);

	if (g_TitleArrived)
	{
		const char* pressText = "Press Z Button ";
		int pressWidth = GetDrawStringWidthToHandle(pressText, strlen(pressText), g_TitleFont);
		int pressX = (640 - pressWidth) / 2;

		int alpha = (int)(g_PressZKeyAlpha * 255);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawStringToHandle(pressX, PRESS_Z_KEY_POS_Y, pressText, GetColor(255, 255, 255), g_TitleFont);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}



void FinTitleScene()
{
	DeleteGraph(g_TitleHandle);
	DeleteGraph(g_PressZKeyHandle);

	// ▼ 解放
	if (g_TitleFont != -1) DeleteFontToHandle(g_TitleFont);

}

