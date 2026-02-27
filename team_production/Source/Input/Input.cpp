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
	// 前回の入力を覚えておく
	g_PrevInputState = g_InputState;

	// 入力状態をクリア
	g_InputState = 0;

	// 入力状態をビットフラグで設定
	if (CheckHitKey(KEY_INPUT_UP))
	{
		g_InputState |= KEY_UP;
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		g_InputState |= KEY_DOWN;
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		g_InputState |= KEY_LEFT;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		g_InputState |= KEY_RIGHT;
	}
	if (CheckHitKey(KEY_INPUT_C))
	{
		g_InputState |= KEY_C;
	}
	if (CheckHitKey(KEY_INPUT_X))
	{
		g_InputState |= KEY_X;
	}
	if (CheckHitKey(KEY_INPUT_Z))
	{
		g_InputState |= KEY_Z;
	}

	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (pad & PAD_INPUT_UP)
	{
		g_InputState |= KEY_UP;
	}
	if (pad & PAD_INPUT_DOWN)
	{
		g_InputState |= KEY_DOWN;
	}
	if (pad & PAD_INPUT_LEFT)
	{
		g_InputState |= KEY_LEFT;
	}
	if (pad & PAD_INPUT_RIGHT)
	{
		g_InputState |= KEY_RIGHT;
	}

	if (pad & PAD_INPUT_A)
	{
		g_InputState |= KEY_Z;
	}
	if (pad & PAD_INPUT_B)
	{
		g_InputState |= KEY_C;
	}
	if (pad & PAD_INPUT_X)
	{
		g_InputState |= KEY_X;
	}

	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		g_InputState |= KEY_ESCAPE;
	}

	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		g_InputState |= KEY_RETURN;
	}

}

void DrawInput()
{
}

void FinInput()
{
}

bool IsInputKey(InputKey key)
{
	return g_InputState & key;
}

bool IsTriggerKey(InputKey key)
{
	return (g_InputState & key) && !(g_PrevInputState & key);
}



