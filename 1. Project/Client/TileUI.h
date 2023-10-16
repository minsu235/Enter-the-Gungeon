#pragma once
#include "UI.h"

#include <Engine\Ptr.h>
#include <Engine\CTexture.h>

class CGameObject;

class TileUI :
    public UI
{
private:
    CGameObject* m_Target;
    Ptr<CTexture>       m_AtlasTex;
    Vec2                m_vTileCount;
    Vec2                m_vSlice;
    int                 TileTargetIdx;
    int                 mousetileIdx;

public:
    void setTarget(CGameObject* _target) { m_Target = _target; }
    void setTexture(Ptr<CTexture> _tex) { m_AtlasTex = _tex; }
    void setCount(Vec2 _count) { m_vTileCount = _count; }
    void setSlice(Vec2 _slice) { m_vSlice = _slice; }

public:
    virtual void update() override;
    virtual void Open() override;
    virtual void render_update() override;
    virtual void Close() override;

public:
    TileUI();
    ~TileUI();
};

