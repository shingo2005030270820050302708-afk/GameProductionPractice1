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

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    ChangeWindowMode(true);
    SetMainWindowText("ロボット工場からの脱出");
    SetWindowPosition(0, 0);
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_DEPTH);

    if (DxLib_Init() == -1) return -1;

    SetTransColor(TRANS_COLOR_R, TRANS_COLOR_G, TRANS_COLOR_B);
    SetDrawScreen(DX_SCREEN_BACK);

    // 背景レイヤー
    BackGround bg;
    bg.AddLayer("Data/BG/1.png");
    bg.AddLayer("Data/BG/2.png");
    bg.AddLayer("Data/BG/3.png");
    bg.AddLayer("Data/BG/4.png");

    InitInput();
    InitPlayer();
    InitBlock();

    // マップ
    LoadMapManager();
    LoadPlayer();
    LoadBlock();
    StartMapManager();
    StartPlayer();
    StartBlock();

    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        UpdateFPS();
        UpdateInput();

        bg.Draw();            // 背景描画

        StepPlayer();
        BlockManager Step;

        UpdatePlayer();
        UpdateMapManager();
        BlockManager Update;
        UpdateBlock(g_PlayerData);

        camera.Update(g_PlayerData);  // カメラ追従

        DrawMapManager();
        DrawPlayer();
        BlockManager Draw;
        DrawBlock();


        FPSWait();
        ScreenFlip();
    }

    // 終了処理
    FinMapManager();
    FinPlayer();
    FinInput();
    FinBlock();

    DxLib_End();
    return 0;
}
