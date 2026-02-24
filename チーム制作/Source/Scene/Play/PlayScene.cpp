#include "PlayScene.h"

void PlayScene::Init() {}
void PlayScene::Load() {}
void PlayScene::Start() {}
void PlayScene::Step() {}
void PlayScene::Update() {}
void PlayScene::Draw() {}
void PlayScene::Fin() {}

bool PlayScene::IsEnd() const { return isEnd; }
SceneType PlayScene::NextScene() const { return next; }//