#pragma once
#include "../SceneBase.h"

class GameClearScene : public SceneBase
{
private:
    bool isEnd = false;         // ★追加：シーン終了フラグ
    SceneType next = SceneType::Title;  // ★追加：次のシーン遷移先

    int clearBG = -1;           // クリア画面の画像
    int timer = 0;              // 表示時間計測用
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
};