#include "DxLib.h"
#include "FPS.h"

#define FPS (60)

static int g_startTime = 0;
static int g_frameStart = 0;
static int g_frameCount = 0;
static float g_fps = 0.0f;

void InitFPS(void)
{
    g_startTime = GetNowCount();
    g_frameStart = g_startTime;
    g_frameCount = 0;
    g_fps = 0.0f;
}

void UpdateFPS(void)
{
    g_frameCount++;
    int now = GetNowCount();
    int elapsed = now - g_startTime;

    if (elapsed >= 1000)
    {
        g_fps = g_frameCount * 1000.0f / elapsed;
        g_frameCount = 0;
        g_startTime = now;
    }
}

void DrawFPS(void)
{
    DrawFormatString(10, 10, GetColor(255, 255, 255), "FPS: %.1f", g_fps);
}

void FPSWait(void)
{
    int tookTime = GetNowCount() - g_frameStart;
    int waitTime = 1000 / FPS - tookTime;

    if (waitTime > 0)
    {
        Sleep(waitTime);
    }

    g_frameStart = GetNowCount();
}
//’Ç‰Á