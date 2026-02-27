#pragma once
#include <memory>
#include "SceneBase.h"

class SceneManager {
private:
    std::unique_ptr<SceneBase> scene;
    SceneType currentScene = SceneType::Title;

public:
    SceneManager(SceneType start);

    void ChangeScene(SceneType next);
    void Update();
    void Draw();
};
//