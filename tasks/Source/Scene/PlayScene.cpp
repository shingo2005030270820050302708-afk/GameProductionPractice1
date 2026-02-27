#include "DxLib.h"
#include "PlayScene.h"
#include "SceneManager.h"



int g_PlayHandle = 0;
int g_PlayerHandle = 0;
int g_Enemy1Handle = 0;
int g_Enemy2Handle = 0;


void InitPlayScene()
{
	g_PlayHandle = 0;
	g_PlayerHandle = 0;
	g_Enemy1Handle = 0;
	g_Enemy2Handle = 0;
}

void LoadPlayScene()
{
	g_PlayHandle = LoadGraph("Data/Play/Play.png");
	g_PlayerHandle = LoadGraph("Data/Play/Player.png");
	g_Enemy1Handle = LoadGraph("Data/Play/Enemy1.png");
	g_Enemy2Handle = LoadGraph("Data/Play/Enemy2.png");
}

void StartPlayScene()
{
}

void StepPlayScene()
{
}

void UpdatePlayScene()
{
	if (CheckHitKey(KEY_INPUT_C))
	{
		ChangeScene(SCENE_TITLE);
	}
}

void DrawPlayScene()
{
	DrawGraph(0, 0, g_PlayHandle, TRUE);
	DrawGraph(1600, 600, g_PlayerHandle, TRUE);
	DrawGraph(250, 200, g_Enemy1Handle, TRUE);
	DrawGraph(350, 550, g_Enemy2Handle, TRUE);
	DrawGraph(375, 590, g_Enemy2Handle, TRUE);
	DrawGraph(325, 650, g_Enemy2Handle, TRUE);
	DrawGraph(415, 685, g_Enemy2Handle, TRUE);
	DrawGraph(450, 750, g_Enemy2Handle, TRUE);
	DrawGraph(500, 700, g_Enemy2Handle, TRUE);
}


void FinPlayScene()
{
	DeleteGraph(g_PlayHandle);
	DeleteGraph(g_PlayerHandle);
	DeleteGraph(g_Enemy1Handle);
	DeleteGraph(g_Enemy2Handle);
}