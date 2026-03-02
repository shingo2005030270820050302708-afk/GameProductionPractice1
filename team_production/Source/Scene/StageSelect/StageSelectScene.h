#pragma once
#include "../SceneBase.h"

class StageSelectScene : public SceneBase {
public:
    void Init() override;
    void Load() override;
    void Start() override;
    void Step() override;
    void Update() override;
    void Draw() override;
    void Fin() override;

    bool IsEnd() const override;
    SceneType NextScene() const override;

private:
    int StageSelectbgHandle = -1;
    int lockIconHandle = -1;
    bool isEnd = false;
    SceneType next = SceneType::Title;
    int cursor = 0;                            // 現在選択中ステージ
    static const int STAGE_MAX = 5;           // ステージ数
    int stageIconHandle[STAGE_MAX];           // ステージアイコン画像
    int cursorHandle;                          // カーソル画像
};//