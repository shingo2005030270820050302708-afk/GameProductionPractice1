#include "Camera.h"

Camera::Camera() : x(0), y(0), screenWidth(1600), screenHeight(900), mapWidth(3200), mapHeight(1800) {}

void Camera::Init(int screenWidth, int screenHeight, int mapWidth, int mapHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
}

void Camera::Update(const PlayerData& player) {
    // 左右スクロール
    if (player.posX - x < scrollMarginX) {
        x = player.posX - scrollMarginX;
    }
    else if (player.posX - x > screenWidth - scrollMarginX) {
        x = player.posX - (screenWidth - scrollMarginX);
    }

    // 上下スクロール
    if (player.posY - y < scrollMarginY) {
        y = player.posY - scrollMarginY;
    }
    else if (player.posY - y > screenHeight - scrollMarginY) {
        y = player.posY - (screenHeight - scrollMarginY);
    }

    // マップ範囲内に制限
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > mapWidth - screenWidth) x = mapWidth - screenWidth;
    if (y > mapHeight - screenHeight) y = mapHeight - screenHeight;
}

Camera camera;
//これ32