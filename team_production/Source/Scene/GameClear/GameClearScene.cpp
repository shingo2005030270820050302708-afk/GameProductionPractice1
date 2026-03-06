#include "GameClearScene.h"
#include <DxLib.h>
#include "../GlobalData.h"

// ステージごとの制限時間 (60FPS)
const int TIME_LIMIT[] =
{
    600,  // Stage1 (10秒)
    900,  // Stage2 (15秒)
    1200, // Stage3 (20秒)
    1500, // Stage4
    1800  // Stage5
};

void GameClearScene::Init() {}

void GameClearScene::Load()
{

    ClearbgHandle = LoadGraph("Data/BG/ClearSceneBG.png");

    

    starHandle = LoadGraph("Data/UI/Star.png");
    starEmptyHandle = LoadGraph("Data/UI/Starempty.png");

    // ステージアンロック
    if (gData.stageNumber >= gData.maxClearStage) {
        gData.maxClearStage = gData.stageNumber + 1;
    }

    timer = 0;

    resultStar = 1;

    int stage = gData.stageNumber;

    if (gData.clearTime <= TIME_LIMIT[stage])
        resultStar++;

    if (gData.coin == 3)
        resultStar++;

    if (resultStar > gData.stageStar[stage])
        gData.stageStar[stage] = resultStar;
}
void GameClearScene::Start() {}
void GameClearScene::Step() {}

void GameClearScene::Update()
{
    timer++;
    starTimer++;

    if (starTimer > 30 && showStar < resultStar)
    {
        showStar++;
        starTimer = 0;
    }

    if (timer > 180) {
        isEnd = true;
        next = SceneType::StageSelect;
    }
}
void GameClearScene::Draw()
{
    SetFontSize(64);

    // 背景描画
    if (ClearbgHandle != -1) {
        DrawGraph(0, 0, ClearbgHandle, TRUE);
    }

    // 星の表示（中央揃え）
    int starSize = 64;
    int spacing = 80;
    int starCount = 3;
    int totalWidth = starSize * starCount + spacing * (starCount - 1);
    int startX = (1600 - totalWidth) / 2;  // 画面中央
    int yStar = 300;

    for (int i = 0; i < starCount; i++)
    {
        if (i < showStar)
            DrawGraph(startX + i * (starSize + spacing), yStar, starHandle, TRUE);
        else
            DrawGraph(startX + i * (starSize + spacing), yStar, starEmptyHandle, TRUE);
    }

    // タイム表示（秒に変換）
    float clearTimeSec = gData.clearTime / 60.0f;
    DrawFormatString(620, yStar + 200, GetColor(0, 0, 0), "TIME : %.1f sec", clearTimeSec);

    // コイン表示
    DrawFormatString(620, yStar + 300, GetColor(0, 0, 0), "COIN : %d / 3", gData.coin);
}
void GameClearScene::Fin() {}

bool GameClearScene::IsEnd() const { return isEnd; }
SceneType GameClearScene::NextScene() const { return next; }