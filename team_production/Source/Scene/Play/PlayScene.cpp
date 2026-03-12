#include "PlayScene.h"
#include "../../Map/MapManager.h"
#include "../../Input/Input.h"
#include "../GlobalData.h"
#include "../../Block/Block.h"
#include "../../Block/WoodBlock.h"
#include "../../Block/BlockHub.h"
#include "../../Enemy/NormalEnemy.h"
#include "../../../Data/Camera/Camera.h"
#include "../../Gimmick/FireFloor.h"

void PlayScene::Init()  
{
    gData.coin = 0;
    clearTime = 0;
    gData.deathCount = 0;
	InitInput();
	InitPlayer();
    InitNormalEnemy();
    InitBlockHub();
	InitFireFloor();
   
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
    LoadNormalEnemy();
    LoadBlockHub();
	LoadFireFloor();
    
}
void PlayScene::Start()
{
    StartPlayer();
    //StartNormalEnemy();
    StartBlockHub();
	StartFireFloor();
       
}
void PlayScene::Step() 
{
    StepPlayer();
    StepNormalEnemy(g_PlayerData);
    StepBlockHub();
	StepFireFloor();
}
void PlayScene::Update()
{
  
    clearTime++;
    UpdatePlayer();
    UpdateBlockHub(g_PlayerData);
    camera.Update(g_PlayerData);
    UpdateMapBlock();
    UpdateFireFloor(g_PlayerData);

    if (CheckHitKey(KEY_INPUT_R))
    {
        isEnd = true;
        next = SceneType::StageSelect;
        return;
    }
	
    // ▼ ここからゴール判定追加 ▼
    if (g_PlayerData.isGoal)
    {
        gData.clearTime = clearTime;
        isEnd = true;
        next = SceneType::GameClear; // クリアシーンへ遷移
    }
}
void PlayScene::Draw()
{
    bg.Draw();
    DrawMapManager();
    DrawPlayer();
    DrawNormalEnemy();
    DrawBlockHub();
	DrawFireFloor();
    char buf[32];
    sprintf_s(buf, "DeathCount: %d", gData.deathCount);
    DrawString(60, 20, buf, GetColor(255, 0, 0));
}
void PlayScene::Fin()
{
    // ▼ ブロックを持っていたら強制的に離す ▼
    if (g_PlayerData.holdingBlock)
    {
        g_PlayerData.holdingBlock = nullptr;
    }

    // ▼ 既存処理 ▼
    FinPlayer();
    FinBlockHub();
}

bool PlayScene::IsEnd() const { return isEnd; }
SceneType PlayScene::NextScene() const { return next; }//