#pragma once

// 入力ボタン定義
enum InputKey
{
    KEY_UP = (1 << 0),
    KEY_DOWN = (1 << 1),
    KEY_LEFT = (1 << 2),
    KEY_RIGHT = (1 << 3),
    KEY_C = (1 << 4),
    KEY_X = (1 << 5),
    KEY_Z = (1 << 6),
    KEY_ESCAPE = (1 << 7),
    KEY_RETURN = (1 << 8),
};
// 関数のプロトタイプ宣言 
void InitInput();
void UpdateInput();
void DrawInput();
void FinInput();

bool IsInputKey(InputKey key);		// キー入力判定
bool IsTriggerKey(InputKey key);	// キー押した瞬間判定




