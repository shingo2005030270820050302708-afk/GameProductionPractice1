#pragma once

// 入力ボタン定義
// 入力ボタン定義
enum InputKey
{
  
    KEY_Z = (1 << 0),      
    KEY_C = (1 << 1),
   
};

// 関数プロトタイプ
void InitInput();
void UpdateInput();
void DrawInput();
void FinInput();

bool IsInputKey(InputKey key);       // キー入力判定
bool IsTriggerKey(InputKey key);     // キー押した瞬間判定
bool IsPressKey(InputKey key);
