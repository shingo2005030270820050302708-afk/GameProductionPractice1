#pragma once

struct GameData {
    int stageNumber = 1;
    int maxClearStage = 0;
    int score = 0;
    bool soundEnabled = true;
};

extern GameData gData;//