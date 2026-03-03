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

    // ★ ロックアイコン（必要なら） ★
    lockIconHandle = LoadGraph("Data/UI/Lock.png");
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

    // カーソル移動（ロックされたステージは選ばない）
    if (IsTriggerKey(KEY_LEFT)) {
        do {
            cursor--;
            if (cursor < 0) cursor = STAGE_MAX - 1;
        } while (cursor > gData.maxClearStage); // ロックされていたらさらに動かす
    }

    if (IsTriggerKey(KEY_RIGHT)) {
        do {
            cursor = (cursor + 1) % STAGE_MAX;
        } while (cursor > gData.maxClearStage); // ロックされていたらさらに動かす
    }

    // 決定（Enter）
    if (IsTriggerKey(KEY_RETURN)) {

        // 未アンロックステージは選べない
        if (cursor > gData.maxClearStage) {
            return; // 遷移しない
        }

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
            x = 150 + i * 550;
            y = 200;
        }
        else {
            x = 400 + (i - 3) * 600;
            y = 550;
        }

        DrawGraph(x, y, stageIconHandle[i], TRUE);

        //  ロック表示（未クリアなら鍵）
        if (i > gData.maxClearStage) {
            DrawGraph(x , y , lockIconHandle, TRUE);
        }

        // 選択中ならカーソル描画（アンロック済みのみ）
        if (i == cursor && i <= gData.maxClearStage) {
            int offsetY = (GetNowCount() / 200) % 2 ? -5 : 0;
            DrawGraph(x - 8, y - 8 + offsetY, cursorHandle, TRUE);
        }
    }
}

void StageSelectScene::Fin()
{

}

bool StageSelectScene::IsEnd() const { return isEnd; }
SceneType StageSelectScene::NextScene() const { return next; }