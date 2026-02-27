#include "TitleScene.h"
#include <DxLib.h>

void TitleScene::Init()
{

}
void TitleScene::Load() 
{
	TitlebgHandle = LoadGraph("Data/BG/TitleBG.png");

}
void TitleScene::Start() 
{

}
void TitleScene::Step()
{

}
void TitleScene::Update() 
{
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        isEnd = true;
        next = SceneType::StageSelect;
    }

}
void TitleScene::Draw() 
{
    if (TitlebgHandle != -1) {
        DrawGraph(0, 0, TitlebgHandle, TRUE);
    }
}
void TitleScene::Fin() 
{

}

bool TitleScene::IsEnd() const { return isEnd; }
SceneType TitleScene::NextScene() const { return next; }//
