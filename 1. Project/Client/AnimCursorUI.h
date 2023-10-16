#pragma once
#include "UI.h"
#include <Engine\Ptr.h>
#include <Engine\CTexture.h>

class AnimUI;
class CAnimation2D;

class AnimCursorUI :
    public UI
{
private:
    Ptr<CTexture>       m_AtlasTex;
    AnimUI* owner;
    float               AnimSlice[2];
    bool                m_check;

public:
    void setTexture(Ptr<CTexture> _tex) { m_AtlasTex = _tex; }
    void setOwner(AnimUI* _owner) { owner = _owner; }
    void setAnimSlice(float _slice[2]) { AnimSlice[0] = _slice[0]; AnimSlice[1] = _slice[1]; }
    void SetOtherTexture(DWORD_PTR _strTexKey);

private:
    Vec4 GetRGBA(int x, int y);

public:
    virtual void update() override;
    virtual void render_update() override;
    virtual void Close() override;

public:
    AnimCursorUI();
    ~AnimCursorUI();
};

