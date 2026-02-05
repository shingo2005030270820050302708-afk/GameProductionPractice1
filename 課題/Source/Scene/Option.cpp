#include "DxLib.h"
#include "SceneManager.h"
#include "../Input/Input.h"



// テキストの位置
#define OPTION_TEXT_POS_X (0)
#define OPTION_TEXT_POS_Y (0)

#define SE_TEXT_POS_X (0)
#define SE_TEXT_POS_Y (0)

#define BGM_TEXT_POS_X (0)
#define BGM_TEXT_POS_Y (0)

// ゲージの位置
#define SE_GAUGE_POSX (0)
#define SE_GAUGE_POSY (0)

#define BGM_GAUGE_POSX (0)
#define BGM_GAUGE_POSY (0)

// テキストハンドル
int g_OptionFont;
int g_BGMFont;
int g_SEFont;
int g_BGMGauge;
int g_SEGauge;

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
	g_OptionFont = LoadGraph("Data/Option/Option.png");
	g_BGMFont = LoadGraph("Data/Option/BGM.png");
	g_SEFont = LoadGraph("Data/Option/SE.png");
	g_BGMGauge = LoadGraph("Data/Option/Gauge.png");
	g_SEGauge = LoadGraph("Data/Option/Gauge.png");
	
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
		// タイトル
		DrawGraph(300, 20, g_OptionFont, TRUE);

		// BGM テキスト
		DrawGraph(100, 150, g_BGMFont, TRUE);
		// BGM ゲージ
		DrawGraph(300, 150, g_BGMGauge, TRUE);

		// SE テキスト
		DrawGraph(100, 300, g_SEFont, TRUE);
		// SE ゲージ
		DrawGraph(300, 300, g_SEGauge, TRUE);
}



void FinOptionScene()
{
	DeleteGraph(g_OptionFont);
	DeleteGraph(g_BGMFont);
	DeleteGraph(g_SEFont);
	DeleteGraph(g_BGMGauge);
	DeleteGraph(g_SEGauge);

}

