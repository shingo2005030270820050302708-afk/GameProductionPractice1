#include "BackGround.h"

BackGround::BackGround() {}
BackGround::~BackGround() { Clear(); }

void BackGround::AddLayer(const std::string& path)
{
    BackGroundLayerData layer;
    layer.handle = LoadGraph(path.c_str());
    layers_.push_back(layer);
}

void BackGround::SetMove(int layerIndex, float moveX, float moveY)
{
    if (layerIndex < 0 || layerIndex >= (int)layers_.size()) return;
    layers_[layerIndex].moveX = moveX;
    layers_[layerIndex].moveY = moveY;
}

void BackGround::Update()
{
    for (auto& layer : layers_)
    {
        layer.posX += layer.moveX;
        layer.posY += layer.moveY;

        // ループスクロール
        if (layer.posX > BG_WIDTH) layer.posX -= BG_WIDTH;
        if (layer.posX < -BG_WIDTH) layer.posX += BG_WIDTH;
        if (layer.posY > BG_HEIGHT) layer.posY -= BG_HEIGHT;
        if (layer.posY < -BG_HEIGHT) layer.posY += BG_HEIGHT;
    }
}

void BackGround::Draw()
{
    int screenW, screenH;
    GetScreenState(&screenW, &screenH, nullptr);

    for (auto& layer : layers_)
    {
        if (layer.handle == 0) continue;

        int startX = (int)layer.posX % BG_WIDTH;
        if (startX > 0) startX -= BG_WIDTH;

        int startY = (int)layer.posY % BG_HEIGHT;
        if (startY > 0) startY -= BG_HEIGHT;

        for (int x = startX; x < screenW; x += BG_WIDTH)
        {
            for (int y = startY; y < screenH; y += BG_HEIGHT)
            {
                DrawGraph(x, y, layer.handle, TRUE);
            }
        }
    }
}

void BackGround::DrawFullScreen(int layerIndex)
{
    if (layerIndex < 0 || layerIndex >= (int)layers_.size()) return;

    int screenW, screenH;
    GetScreenState(&screenW, &screenH, nullptr);

    DrawExtendGraph(
        0, 0,
        screenW, screenH,
        layers_[layerIndex].handle,
        TRUE
    );
}

void BackGround::Clear()
{
    for (auto& layer : layers_)
    {
        if (layer.handle != 0)
        {
            DeleteGraph(layer.handle);
            layer.handle = 0;
        }
    }
    layers_.clear();
}