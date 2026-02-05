#include "DxLib.h"
#include "PlayScene.h"
#include "SceneManager.h"



int g_PlayHandle = 0;
int g_PlayerHandle = 0;
int g_EnemyHandle = 0;

void InitPlayScene()
{
	g_PlayHandle = 0;
	g_PlayerHandle = 0;
	g_EnemyHandle = 0;
}

void LoadPlayScene()
{
	g_PlayHandle = LoadGraph("Data/.png");
	g_PlayerHandle = LoadGraph("Data/Player.png");
	g_EnemyHandle = LoadGraph("Data/Enemy.png");
}

void StartPlayScene()
{
}

void StepPlayScene()
{
}

void UpdatePlayScene()
{
}

void DrawPlayScene()
{
	DrawGraph(0, 0, g_PlayHandle, TRUE);
	DrawGraph(200, 35, g_PlayerHandle, TRUE);
	DrawGraph(900, 25, g_EnemyHandle, TRUE);
}

void FinPlayScene()
{
	DeleteGraph(g_PlayHandle);
	DeleteGraph(g_PlayerHandle);
	DeleteGraph(g_EnemyHandle);
}