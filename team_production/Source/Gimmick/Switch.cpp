#include "Switch.h"
#include "../../Data/Camera/Camera.h"
#include "../Block/BlockHub.h"

SwitchData g_Switch[SWITCH_MAX];

int g_SwitchHandleNormal = -1;
int g_SwitchHandlePressed = -1;

// 初期化
void InitSwitch()
{
    for (int i = 0; i < SWITCH_MAX; ++i)
    {
        g_Switch[i].active = false;
        g_Switch[i].pressed = false;
        g_Switch[i].linkedMapBlockIndex = -1;
        g_Switch[i].width = 32.0f;
        g_Switch[i].height = 8.0f;
        g_Switch[i].pos = VGet(0, 0, 0);
        g_Switch[i].handleNormal = -1;
        g_Switch[i].handlePressed = -1;
    }
}

// 画像読み込み
void LoadSwitch()
{
    
     g_Switch[0].handleNormal = LoadGraph("Data/Gimmick/Switch_Normal.png");
     g_Switch[0].handlePressed = LoadGraph("Data/Gimmick/Switch_Pressed.png");
}

// Switch.cpp 側
SwitchData* CreateSwitch(int mapX, int mapY, int blockIndex,
    int handleNormal, int handlePressed)
{
    for (int i = 0; i < SWITCH_MAX; ++i)
    {
        if (!g_Switch[i].active)
        {
            g_Switch[i].active = true;
            float SWITCH_OFFSET_Y = 15.0f; // 下方向にずらす量

            g_Switch[i].pos = VGet(mapX * 32.0f, mapY * 32.0f + SWITCH_OFFSET_Y, 0);
            g_Switch[i].width = 32.0f;
            g_Switch[i].height = 20.0f;
            g_Switch[i].linkedMapBlockIndex = blockIndex;
            g_Switch[i].handleNormal = handleNormal;
            g_Switch[i].handlePressed = handlePressed;
            return &g_Switch[i];
        }
    }
    return nullptr;
}
void UpdateSwitches(PlayerData& player)
{
    float margin = 10.0f; // 判定に猶予

    for (int i = 0; i < SWITCH_MAX; ++i)
    {
        SwitchData& s = g_Switch[i];
        if (!s.active) continue;

        s.pressed = false;

        // プレイヤー自身
        float px = player.posX + PLAYER_BOX_COLLISION_OFFSET_X;
        float py = player.posY + PLAYER_BOX_COLLISION_OFFSET_Y;
        float pw = player.boxCollision.width;
        float ph = player.boxCollision.height;

        if (px + pw > s.pos.x && px < s.pos.x + s.width &&
            py + ph > s.pos.y && py < s.pos.y + s.height)
        {
            s.pressed = true;
        }

        // プレイヤーが持っているブロック
        if (player.holdingBlock)
        {
            BlockData& b = *player.holdingBlock;
            if (b.pos.x + b.width + margin > s.pos.x &&
                b.pos.x - margin < s.pos.x + s.width &&
                b.pos.y + b.height + margin > s.pos.y &&
                b.pos.y - margin < s.pos.y + s.height)
            {
                s.pressed = true;
            }
        }

        // 置かれたブロック（BLOCK_STAY / BLOCK_THROW も対象）
        for (int bi = 0; bi < BLOCK_MAX; ++bi)
        {
            BlockData& b = g_Block[bi];
            if (!b.active) continue;

            if (b.pos.x + b.width + margin > s.pos.x &&
                b.pos.x - margin < s.pos.x + s.width &&
                b.pos.y + b.height + margin > s.pos.y &&
                b.pos.y - margin < s.pos.y + s.height)
            {
                s.pressed = true;
                break;
            }
        }

        // IceBlock
        for (int bi = 0; bi < BLOCK_MAX; ++bi)
        {
            BlockData& b = g_IceBlock[bi];
            if (!b.active) continue;

            if (b.pos.x + b.width + margin > s.pos.x &&
                b.pos.x - margin < s.pos.x + s.width &&
                b.pos.y + b.height + margin > s.pos.y &&
                b.pos.y - margin < s.pos.y + s.height)
            {
                s.pressed = true;
                break;
            }
        }

        // マップブロックのアクティブ化
        if (s.linkedMapBlockIndex >= 0)
            g_MapBlocks[s.linkedMapBlockIndex].active = s.pressed;
    }
}
// 描画
void DrawSwitches()
{
    extern Camera camera;

    for (int i = 0; i < SWITCH_MAX; ++i)
    {
        SwitchData& s = g_Switch[i];
        if (!s.active) continue;

        // 画像描画
        if (s.pressed && s.handlePressed != -1)
        {
            DrawGraph((int)(s.pos.x - camera.GetX()),
                      (int)(s.pos.y - camera.GetY()),
                      s.handlePressed, TRUE);
        }
        else if (!s.pressed && s.handleNormal != -1)
        {
            DrawGraph((int)(s.pos.x - camera.GetX()),
                      (int)(s.pos.y - camera.GetY()),
                      s.handleNormal, TRUE);
        }
        else
        {
            // 画像がない場合は箱で描画
            DrawBox((int)(s.pos.x - camera.GetX()),
                    (int)(s.pos.y - camera.GetY()),
                    (int)(s.pos.x + s.width - camera.GetX()),
                    (int)(s.pos.y + s.height - camera.GetY()),
                    s.pressed ? GetColor(0, 255, 0) : GetColor(255, 0, 0), TRUE);
        }
    }
}