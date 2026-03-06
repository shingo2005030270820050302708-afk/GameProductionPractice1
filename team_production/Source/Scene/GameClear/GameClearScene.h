#pragma once
#include "../SceneBase.h"

class GameClearScene : public SceneBase
{
private:
    bool isEnd = false;
    SceneType next = SceneType::Title;
    int ClearbgHandle = -1;
    int clearBG = -1;
    int timer = 0;
    int starHandle = -1;
    int starEmptyHandle = -1;

    int resultStar = 0;  
    int showStar = 0;
    int starTimer = 0;

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