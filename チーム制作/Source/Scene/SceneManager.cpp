#include "SceneManager.h"
#include "Title/TitleScene.h"
#include "StageSelect/StageSelectScene.h"
#include "Play/PlayScene.h"
#include "Setting/SettingScene.h"
#include "GameClear/GameClearScene.h"
#include "GameOver/GameOverScene.h"

SceneManager::SceneManager(SceneType start)
{
    ChangeScene(start);
}

void SceneManager::ChangeScene(SceneType next) 
{
    currentScene = next;

    switch (next) {
    case SceneType::Title:
        scene = std::make_unique<TitleScene>();
        break;

    case SceneType::StageSelect:
        scene = std::make_unique<StageSelectScene>();
        break;

    case SceneType::Play:
        scene = std::make_unique<PlayScene>();
        break;

    case SceneType::Setting:
        scene = std::make_unique<SettingScene>();
        break;

    case SceneType::GameClear:
        scene = std::make_unique<GameClearScene>();
        break;

    case SceneType::GameOver:
        scene = std::make_unique<GameOverScene>();
        break;
    }
    scene->Init();
    scene->Load();
    scene->Start();
}

void SceneManager::Update() 
{
    scene->Step();
    scene->Update();

    if (scene->IsEnd()) 
    {
        SceneType next = scene->NextScene();
        scene->Fin();
        ChangeScene(next);
    }
}

void SceneManager::Draw() 
{
    scene->Draw();
}
