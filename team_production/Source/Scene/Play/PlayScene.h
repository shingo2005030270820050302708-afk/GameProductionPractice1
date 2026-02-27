#pragma once
#include "../SceneBase.h"
#include "../../BackGround/BackGround.h"

class PlayScene : public SceneBase {
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
    BackGround bg;
    bool isEnd = false;
    SceneType next = SceneType::Title;
};//