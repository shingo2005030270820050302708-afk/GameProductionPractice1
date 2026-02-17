#pragma once
#include "DxLib.h"
#include <vector>
#include <string>

struct BackGroundLayerData
{
    int handle = 0;
    float posX = 0.0f;
    float posY = 0.0f;
    float moveX = 0.0f;
    float moveY = 0.0f;
};

class BackGround
{
public:
    BackGround();
    ~BackGround();

    // レイヤー追加（ロードもここでやる）
    void AddLayer(const std::string& path);

    // 移動量を設定（自動スクロール用）
    void SetMove(int layerIndex, float moveX, float moveY);

    // 更新・描画
    void Update();
    void Draw();
    void DrawFullScreen(int layerIndex);

    // 解放
    void Clear();

private:
    std::vector<BackGroundLayerData> layers_;
    const int BG_WIDTH = 1600;
    const int BG_HEIGHT = 900;
};