#include "PlayScene.h"
#include "../../Map/MapManager.h"
#include "../../Input/Input.h"
#include "../../Block/Block.h"
#include "../GlobalData.h"

void PlayScene::Init()  
{
	InitInput();
	InitPlayer();
	InitBlock();
}
void PlayScene::Load()
{
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
}
void PlayScene::Start()
{}
void PlayScene::Step() 
{

}
void PlayScene::Update() 
{

}
void PlayScene::Draw()
{
	DrawMapManager();
}
void PlayScene::Fin()
{

}

bool PlayScene::IsEnd() const { return isEnd; }
SceneType PlayScene::NextScene() const { return next; }