#pragma once
#include "ComponentUI.h"

#include <Engine\Ptr.h>
#include <Engine\CTexture.h>

class CAnimation2D;

class Animator2DUI :
    public ComponentUI
{
private:
    CAnimation2D* m_Animation;
    Ptr<CTexture>       m_AtlasTex;
    float               m_AccTime;
    int                 m_iCurIdx;

public:
    virtual void update() override;
    virtual void render_update() override;
    virtual void Close() override { ComponentUI::Close(); m_Animation = nullptr; m_AccTime = 0.f; m_iCurIdx = 0; }
    void setzero() { m_iCurIdx = 0; }

private:
    void SetAnimaion(DWORD_PTR _strAnimKey);
    void SaveAnimation();

public:
    Animator2DUI();
    ~Animator2DUI();
};

