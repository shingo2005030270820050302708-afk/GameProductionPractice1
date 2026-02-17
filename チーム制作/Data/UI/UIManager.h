#pragma once
#include "DxLib.h"

class UIManager
{
public:
    UIManager();
    ~UIManager();

    void Load();    
    void Update();  
    void Draw();    
    void Fin();     

private:
    int UIhandle;   
    int posx;     
    int posy;       
};