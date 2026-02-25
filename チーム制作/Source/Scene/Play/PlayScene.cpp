#include "PlayScene.h"
#include "../../Map/MapManager.h"
#include "../../Input/Input.h"
#include "../../Block/Block.h"

void PlayScene::Init()  
{
	InitInput();
	InitPlayer();
	InitBlock();
}
void PlayScene::Load() 
{
	LoadMapManager();
}
void PlayScene::Start()
{}
void PlayScene::Step() 
{

}
void PlayScene::Update() 
{

}
void PlayScene::Draw()
{
	DrawMapManager();
}
void PlayScene::Fin()
{

}

bool PlayScene::IsEnd() const { return isEnd; }
SceneType PlayScene::NextScene() const { return next; }//