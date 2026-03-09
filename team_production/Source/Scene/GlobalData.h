#pragma once

struct GameData {
    int stageNumber = 1;
    int maxClearStage = 0;
	int deathCount = 0;
    int coin = 0;
    int clearTime = 0;
    bool soundEnabled = true;
    int stageStar[10] = { 0 };
  
};

extern GameData gData;//