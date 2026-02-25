#pragma once
#include "../SceneBase.h"

class TitleScene : public SceneBase {
public:
    TitleScene() = default;
    ~TitleScene() override = default;

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
    int TitlebgHandle = -1;
    bool isEnd = false;
    SceneType next = SceneType::Title;
};
