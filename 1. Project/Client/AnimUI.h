#pragma once
#include "UI.h"

#include <Engine\Ptr.h>
#include <Engine\CTexture.h>

class Animator2DUI;
class CAnimation2D;

class AnimUI :
    public UI
{
private:
    CAnimation2D* m_Animation;
    Ptr<CTexture>       m_AtlasTex;
    float               m_AccTime;
    int                 m_iCurIdx;
    int                 m_focusIdx;
    vector<tAnim2DFrm>  m_copyvector;
    bool                m_bPlay;
    int                 AnimUIFps;
    float               Animfullsize[2];
    float               Animlefttop[2];
    float               Animoffset[2];
    float               AnimSlice[2];
    float               AnimStep;

    bool                m_bStepAnimation;
    Animator2DUI* owner;

public:
    void setTarget(CAnimation2D* _target) { m_Animation = _target; }
    void setTexture(Ptr<CTexture> _tex) { m_AtlasTex = _tex; }
    void setVector(vector<tAnim2DFrm> _copyvec);
    void setOwner(Animator2DUI* _owner) { owner = _owner; }
    void setAnimlefttop(float _lefttop[2]) { m_copyvector[m_iCurIdx].vLeftTop = Vec2(_lefttop[0] / m_AtlasTex->GetWidth(), _lefttop[1] / m_AtlasTex->GetHeight()); }
    void setAnimSlice(float _slice[2]) { m_copyvector[m_iCurIdx].vSlice = Vec2(_slice[0] / m_AtlasTex->GetWidth(), _slice[1] / m_AtlasTex->GetHeight()); }
public:
    virtual void update() override;
    virtual void render_update() override;
    virtual void Close() override;

public:
    AnimUI();
    ~AnimUI();

};

