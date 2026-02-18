#pragma once
#include "DxLib.h"
#include "../../Source/Player/Player.h"

class Camera {
public:
    Camera();

    void Init(int screenWidth, int screenHeight, int mapWidth, int mapHeight);
    void Update(const PlayerData& player); // PlayerDataを渡す

    int GetX() const { return (int)x; }
    int GetY() const { return (int)y; }

private:
    float x, y;
    int screenWidth;
    int screenHeight;
    int mapWidth;
    int mapHeight;

    int scrollMarginX = 400; // 左右スクロール余白
    int scrollMarginY = 200; // 上下スクロール余白
};

extern Camera camera;
//l3