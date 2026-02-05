#include "SceneManager.h"
#include <DxLib.h>
#include "TitleScene.h"


Scene g_NowScene = SCENE_TITLE;
Scene g_NextScene = SCENE_TITLE;
SceneState g_SceneState = SCENE_STATE_INIT;
bool g_IsLoopEnd = false;


void SceneManagerUpdate()
{
	switch (g_SceneState)
	{
	case SCENE_STATE_INIT:
		InitScene();
		g_SceneState = SCENE_STATE_LOAD;
		break;

	case SCENE_STATE_LOAD:
		LoadScene();
		g_SceneState = SCENE_STATE_START;
		break;

	case SCENE_STATE_START:
		StartScene();
		g_SceneState = SCENE_STATE_LOOP;
		break;

	case SCENE_STATE_LOOP:
		StepScene();
		UpdateScene();
		DrawScene();
		if (g_IsLoopEnd)
		{
			g_SceneState = SCENE_STATE_FIN;
		}
		break;

	case SCENE_STATE_FIN:
		FinScene();

		if (g_IsLoopEnd)
		{
			g_NowScene = g_NextScene;
			g_SceneState = SCENE_STATE_INIT;
			g_IsLoopEnd = false;
		}
		break;


	}
}
void InitScene()
{
	switch (g_NowScene)
	{
	case SCENE_TITLE: InitTitleScene(); break;
	case SCENE_PLAY: InitPlayScene(); break;
	case SCENE_OPTION: Init(); break;
	}
}

void LoadScene()
{
	switch (g_NowScene)
	{
	case SCENE_TITLE: LoadTitleScene(); break;
	case SCENE_PLAY: LoadPlayScene(); break;
	case SCENE_OPTION: Load(); break;
	}
}

void StartScene()
{
	switch (g_NowScene)
	{
	case SCENE_TITLE: StartTitleScene(); break;
	case SCENE_PLAY: StartPlayScene(); break;
	case SCENE_OPTION: Start(); break;
	}
}

void StepScene()
{
	switch (g_NowScene)
	{
	case SCENE_TITLE: StepTitleScene(); break;
	case SCENE_PLAY: StepPlayScene(); break;
	case SCENE_OPTION: Step(); break;
	}
}

void UpdateScene()
{
	switch (g_NowScene)
	{
	case SCENE_TITLE: UpdateTitleScene(); break;
	case SCENE_PLAY: UpdatePlayScene(); break;
	case SCENE_OPTION: Update(); break;
	}
}

void DrawScene()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);
	SetDrawMode(DX_DRAWMODE_NEAREST);

	switch (g_NowScene)
	{
	case SCENE_TITLE:
		DrawTitleScene();
		break;

	case SCENE_OPTION:
		DrawGame();
		break;
	case SCENE_PLAY:
		DrawPlayScene();
		break;
	}
}


void FinScene()
{
	switch (g_NowScene)
	{
	case SCENE_TITLE: FinTitleScene(); break;
	case SCENE_PLAY: FinPlayScene(); break;
	case SCENE_OPTION: FinGameOverScene(); break;
	
	}
}
void ChangeScene(Scene scene)
{

	g_NextScene = scene;


	g_IsLoopEnd = true;
}

