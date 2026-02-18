#include "UIBase.h"

UIBase::UIBase()
{
    pos_x = 0;
    pos_y = 0;
    active = true;
}

UIBase::~UIBase()
{
}

void UIBase::Load()
{
}

void UIBase::Update()
{
}

void UIBase::Draw()
{
}

void UIBase::SetPos(int x, int y)
{
    pos_x = x;
    pos_y = y;
}

void UIBase::SetActive(bool flag)
{
    active = flag;
}

bool UIBase::IsActive() const
{
    return active;
}
