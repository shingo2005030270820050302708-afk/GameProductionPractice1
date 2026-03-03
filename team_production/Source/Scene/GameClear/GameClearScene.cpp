#include "GameClearScene.h"
#include <DxLib.h>
#include "../GlobalData.h"
void GameClearScene::Init() {}

void GameClearScene::Load()
{
    clearBG = LoadGraph("Data/BG/GameClear.png");

    // ステージアンロック
    if (gData.stageNumber >= gData.maxClearStage) {
        gData.maxClearStage = gData.stageNumber + 1;
    }

    timer = 0;
}

void GameClearScene::Start() {}
void GameClearScene::Step() {}

void GameClearScene::Update()
{
    timer++;

    // 3秒後にステージセレクトへ戻る
    if (timer > 180) {
        isEnd = true;
        next = SceneType::StageSelect;
    }
}

void GameClearScene::Draw()
{
    if (clearBG != -1) {
        DrawGraph(0, 0, clearBG, TRUE);
    }

    DrawString(700, 300, "STAGE CLEAR!", GetColor(255, 255, 0));
    DrawString(650, 650, "Back to Stage Select...", GetColor(255, 255, 255));
}

void GameClearScene::Fin() {}

bool GameClearScene::IsEnd() const { return isEnd; }
SceneType GameClearScene::NextScene() const { return next; }