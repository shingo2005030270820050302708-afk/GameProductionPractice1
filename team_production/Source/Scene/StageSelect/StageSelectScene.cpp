#include "StageSelectScene.h"
#include <DxLib.h>
#include "../Title/TitleScene.h"
#include "../GlobalData.h"
#include "../../Input/Input.h"

void StageSelectScene::Init() 
{

}
void StageSelectScene::Load() 
{
    StageSelectbgHandle = LoadGraph("Data/BG/StageSelectBG.png");

    stageIconHandle[0] = LoadGraph("Data/UI/StageIcon1.png");
    stageIconHandle[1] = LoadGraph("Data/UI/StageIcon1.png");
    stageIconHandle[2] = LoadGraph("Data/UI/StageIcon1.png");
    stageIconHandle[3] = LoadGraph("Data/UI/StageIcon1.png");
    stageIconHandle[4] = LoadGraph("Data/UI/StageIcon1.png");
    cursorHandle = LoadGraph("Data/UI/Cursor.png");
}
void StageSelectScene::Start()
{

}
void StageSelectScene::Step() 
{

}
void StageSelectScene::Update()
{
    // ESCでタイトルに戻る
    if (IsTriggerKey(KEY_ESCAPE)) {
        isEnd = true;
        next = SceneType::Title;
        }

    // カーソル移動
    if (IsTriggerKey(KEY_LEFT)) {
        cursor--;
        if (cursor < 0) cursor = STAGE_MAX - 1;
    }
    if (IsTriggerKey(KEY_RIGHT)) {
        cursor = (cursor + 1) % STAGE_MAX;
    }

    // 決定（Enter）
    if (IsTriggerKey(KEY_RETURN)) {
        gData.stageNumber = cursor;
        isEnd = true;
        next = SceneType::Play;
    }
}
void StageSelectScene::Draw()
{
    // 背景描画
    if (StageSelectbgHandle != -1) {
        DrawGraph(0, 0, StageSelectbgHandle, TRUE);
    }

    // ステージアイコン描画（上3個、下2個）
    for (int i = 0; i < StageSelectScene::STAGE_MAX; i++) {
        int x, y;

        if (i < 3) {
            // 上段3個
            x = 150 + i * 550;   // 横位置
            y = 200;             // 上段の縦位置
        }
        else {
            // 下段2個
            x = 400  + (i - 3) * 600;  // 横位置（調整可）
            y = 550;                   // 下段の縦位置
        }

        // アイコン描画
        DrawGraph(x, y, stageIconHandle[i], TRUE);

        // 選択中なら枠を描画
        if (i == cursor) {
            int offsetY = (GetNowCount() / 200) % 2 ? -5 : 0; // 上下揺れ
            DrawGraph(x - 8, y - 8 + offsetY, cursorHandle, TRUE);
        }
    }
}
 void StageSelectScene::Fin()
{

}

bool StageSelectScene::IsEnd() const { return isEnd; }
SceneType StageSelectScene::NextScene() const { return next; }//