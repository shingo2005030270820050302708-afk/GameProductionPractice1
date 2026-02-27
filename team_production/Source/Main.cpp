#include "DxLib.h"
#include "Map/MapManager.h"
#include "GameSetting/GameSetting.h"
#include "BackGround/BackGround.h"
#include "Player/Player.h"
#include "Input/Input.h"
#include "FPS/Fps.h"
#include "../Data/Camera/Camera.h"
#include "Block/Block.h"
#include "Block/BlockManager.h"
#include "Scene/SceneManager.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    ChangeWindowMode(false);
    SetMainWindowText("ロボット工場からの脱出");
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_DEPTH);
    if (DxLib_Init() == -1) return -1;

    SetDrawScreen(DX_SCREEN_BACK);

    SceneManager sceneManager(SceneType::Title); // タイトルシーンから開始

    while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_E))
    {
        ClearDrawScreen();

        UpdateFPS();
        UpdateInput();

        sceneManager.Update();  // シーン更新と自動切替
        sceneManager.Draw();    // シーン描画

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
//