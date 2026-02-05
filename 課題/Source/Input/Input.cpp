#include "DxLib.h"
#include "Input.h"

// 入力ビットフラグ
int g_InputState = 0;
// 前回の入力ビット
int g_PrevInputState = 0;

void InitInput()
{
    g_InputState = 0;
    g_PrevInputState = 0;
}

void UpdateInput()
{
    g_PrevInputState = g_InputState;
    g_InputState = 0;

    // -------------------
    // キーボード入力
  
    if (CheckHitKey(KEY_INPUT_Z))     g_InputState |= KEY_Z;
 
}

void DrawInput()
{
    // 特に描画なし
}

void FinInput()
{
    // 特に終了処理なし
}

bool IsInputKey(InputKey key)
{
    return (g_InputState & key) != 0;
}

bool IsTriggerKey(InputKey key)
{
    return ((g_InputState & key) != 0) && ((g_PrevInputState & key) == 0);
}

bool IsPressKey(InputKey key)
{
    return (g_InputState & key) != 0;
}

