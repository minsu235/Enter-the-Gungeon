#pragma once
#include "ComponentUI.h"
class CameraUI :
    public ComponentUI
{
private:
    Vec3            m_vPos;
    Vec3            m_vRot;
    float           m_fScale;
    float           m_fFar;
    int             m_proj_item_current;

private:
    virtual void update() override;
    virtual void render_update() override;

public:
    CameraUI();
    ~CameraUI();
};

