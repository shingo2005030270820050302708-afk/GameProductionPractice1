#pragma once

class UIBase
{
public:
    UIBase();
    virtual ~UIBase();

    virtual void Load();
    virtual void Update();
    virtual void Draw();

    void SetPos(int x, int y);
    void SetActive(bool flg);
    bool IsActive() const;

protected:
    int pos_x;
    int pos_y;
    bool active;
};
//43