#include "PlayScene.h"
#include "../../Map/MapManager.h"
#include "../../Input/Input.h"
#include "../GlobalData.h"
#include "../../Block/Block.h"
#include "../../Block/WoodBlock.h"

void PlayScene::Init()  
{
	InitInput();
	InitPlayer();
    InitBlock();
}
void PlayScene::Load()
{

    switch (gData.stageNumber)
    {
    case 0:
        bg.AddLayer("Data/BG/1.png");
        bg.AddLayer("Data/BG/2.png");
        bg.AddLayer("Data/BG/3.png");
        bg.AddLayer("Data/BG/4.png");
        break;
    case 1:
        bg.AddLayer("Data/BG/1.png");
        bg.AddLayer("Data/BG/2.png");
        bg.AddLayer("Data/BG/3.png");
        bg.AddLayer("Data/BG/4.png");
        break;
    case 2:
        bg.AddLayer("Data/BG/1.png");
        bg.AddLayer("Data/BG/2.png");
        bg.AddLayer("Data/BG/3.png");
        bg.AddLayer("Data/BG/4.png");
        break;
    }
    const char* mapPath = nullptr;

    switch (gData.stageNumber)
    {
    case 0: mapPath = "Data/Map/Stage1.bin"; break;
    case 1: mapPath = "Data/Map/Stage2.bin"; break;
    case 2: mapPath = "Data/Map/Stage3.bin"; break;
    case 3: mapPath = "Data/Map/Stage4.bin"; break;
    case 4: mapPath = "Data/Map/Stage5.bin"; break;
    default: mapPath = "Data/Map/DefaultStage.bin"; break;
    }

    LoadMapManager(mapPath); // ステージに応じたマップ読み込み

    LoadPlayer();
    LoadBlock();
    LoadWoodBlock();
}
void PlayScene::Start()
{
    StartPlayer();
    StartBlock();
    StartWoodBlock();
}
void PlayScene::Step() 
{
    StepPlayer();
    StepBlock();
    StepWoodBlock();
}
void PlayScene::Update() 
{
    UpdatePlayer();
    UpdateBlock(g_PlayerData);
    UpdateWoodBlock(g_PlayerData);
}
void PlayScene::Draw()
{
    bg.Draw();
	DrawMapManager();
    DrawPlayer();
    DrawBlock();
    DrawWoodBlock();
}
void PlayScene::Fin()
{
    FinPlayer();
    FinBlock();
}

bool PlayScene::IsEnd() const { return isEnd; }
SceneType PlayScene::NextScene() const { return next; }//