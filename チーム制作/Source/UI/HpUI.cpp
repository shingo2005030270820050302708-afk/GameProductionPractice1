#include "HpUI.h"
#include "DxLib.h"

HpUI::HpUI(int maxHP, int spacing)
{
    pos_x = 0;
    pos_y = 0;
    active = true;

    m_maxHP = maxHP;
    m_currentHP = maxHP;
    m_spacing = spacing;

    m_fullHeartHandle = -1;
    m_emptyHeartHandle = -1;
}

HpUI::~HpUI()
{
    if (m_fullHeartHandle != -1) DeleteGraph(m_fullHeartHandle);
    if (m_emptyHeartHandle != -1) DeleteGraph(m_emptyHeartHandle);
}

void HpUI::Load()
{
    m_fullHeartHandle = LoadGraph("Data/UI/hp.png");
    m_emptyHeartHandle = LoadGraph("Data/UI/emptyhp.png");
}

void HpUI::SetHP(int hp)
{
    m_currentHP = hp;
    if (m_currentHP < 0) m_currentHP = 0;
    if (m_currentHP > m_maxHP) m_currentHP = m_maxHP;
}

void HpUI::Update()
{
    if (!active) return;
}

void HpUI::Draw()
{
    if (!active) return;

    for (int i = 0; i < m_maxHP; i++)
    {
        int x = pos_x + i * m_spacing;
        int y = pos_y;

        if (i < m_currentHP)
        {
            DrawGraph(x, y, m_fullHeartHandle, true);
        }
        else
        {
            DrawGraph(x, y, m_emptyHeartHandle, true);
        }
    }
}
//67