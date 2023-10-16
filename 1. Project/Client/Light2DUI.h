#pragma once
#include "ComponentUI.h"
class Light2DUI :
    public ComponentUI
{

private:
    float           m_vColor[4];
    int           m_Radius;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    Light2DUI();
    ~Light2DUI();
};

