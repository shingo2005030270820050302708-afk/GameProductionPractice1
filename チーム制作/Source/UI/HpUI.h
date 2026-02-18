#pragma once
#include "UIBase.h"

class HpUI : public UIBase
{
public:
    HpUI(int maxHP = 3, int spacing = 32);     // HPÅ‘å’l‚Æ‰æ‘œ‚ÌŠÔŠu
    virtual ~HpUI();

    void SetHP(int hp);
    void Load() override;
    void Update() override;
    void Draw() override;

private:
    int m_fullHeartHandle;
    int m_emptyHeartHandle;

    int m_spacing;
    int m_currentHP;
    int m_maxHP;
};
