#pragma once
#include "../Player/Player.h"

class GimmickManager
{
public:
    void InitGimmickManager();
    void LoadGimmickManager();
    void StartGimmickManager();
    void StepGimmickManager();
    void UpdateGimmickManager(PlayerData& player);
    void DrawGimmickManager();
    void FinGimmickManager();
};