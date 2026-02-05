#include <DxLib.h>
#include "GameSetting/GameSetting.h"
#include "Scene/SceneManager.h"
#include "Input/Input.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    ChangeWindowMode(true);
   

    if (DxLib_Init() == -1) return -1;

    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTransColor(TRANS_COLOR_R, TRANS_COLOR_G, TRANS_COLOR_B);
    SetDrawScreen(DX_SCREEN_BACK);

   

    while (ProcessMessage() >= 0)
    {
        Sleep(1);
        ClearDrawScreen();
        UpdateInput();
        SceneManagerUpdate();
        DrawInput();
        if (CheckHitKey(KEY_INPUT_ESCAPE)) break;
        DrawExtendGraph(0, 0, SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4, GetDrawScreen(), FALSE);
        ScreenFlip();
    }

   

    DxLib_End();
    return 0;
}
