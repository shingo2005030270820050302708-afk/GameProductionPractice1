#include "DxLib.h"
#include "TitleScene.h"
#include "../Scene/Option.h"
#include <math.h>
#include "SceneManager.h"
#include "../Input/Input.h"

// 背景の位置
#define BG_POS_X (0)
#define BG_POS_Y (0)

// テキストの位置
#define OPTION_TEXT_POS_X (40)
#define OPTION_TEXT_POS_Y (40)

#define SE_TEXT_POS_X (20)
#define SE_TEXT_POS_Y (60)

#define BGM_TEXT_POS_X (20)
#define BGM_TEXT_POS_Y (85)

// ゲージの位置
#define SE_GAUGE_POSX (45)
#define SE_GAUGE_POSY (60)

#define BGM_GAUGE_POSX (45)
#define BGM_GAUGE_POSY (85)

// テキストハンドル
int g_OptionFont = -1;
int g_BGMFont = -1;
int g_SEFont = -1;
int g_BGMGauge = -1;
int g_SEGauge = -1;



void InitOptionScene()
{
	g_BGMFont = 0;
	g_SEFont = 0;
	g_OptionFont = 0.0f;
	g_BGMGauge = 0.0f;
	g_SEGauge = 0.0f;
}


void LoadOptionScene()
{
	int g_BGHandle = LoadGraph("Date/BG.png");
	int g_OptionFont = LoadGraph("Date/Option.png");
	int g_BGMFont = LoadGraph("Date/BGM.png");
	int g_SEFont = LoadGraph("Date/SE.png");
	int g_BGMGauge = LoadGraph("Date/Gauge.png");
	int g_SEGauge = LoadGraph("Date/Gauge.png");
}


void StartOptionScene()
{

}


void StepOptionScene()
{

}


void UpdateOptionScene()
{
	if (CheckHitKey(KEY_INPUT_C))
	{
		ChangeScene(SCENE_TITLE);
	}
}

void DrawOptionScene()
{


}



void FinOptionScene()
{


}