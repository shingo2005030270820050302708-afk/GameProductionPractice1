#include "DxLib.h"
#include "Map/MapManager.h"
#include "GameSetting/GameSetting.h"
#include "BackGround/BackGround.h"
#include "Player/Player.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    
    ChangeWindowMode(true);       
    SetMainWindowText("ロボット工場からの脱出");//タブの名前
    SetWindowPosition(0, 0);//ウィンドウの初期位置
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_DEPTH);//ウィンドウの幅
    if (DxLib_Init() == -1) return -1;
    SetTransColor(TRANS_COLOR_R, TRANS_COLOR_G, TRANS_COLOR_B);
    SetDrawScreen(DX_SCREEN_BACK);

    // 背景レイヤー
    BackGround bg;
    bg.AddLayer("Data/BG/1.png");
    bg.AddLayer("Data/BG/2.png");
    bg.AddLayer("Data/BG/3.png");
    bg.AddLayer("Data/BG/4.png");

    // マップ
    LoadMapManager();
    LoadPlayer();
    
    StartMapManager();
    StartPlayer();
    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        bg.Draw();            // 背景描画
        UpdateMapManager();   // マップ更新
        UpdatePlayer();
        DrawMapManager(); 
        DrawPlayer();

        ScreenFlip();
    }

    // 終了処理
    FinMapManager();
    FinPlayer();
    DxLib_End();
    return 0;
}