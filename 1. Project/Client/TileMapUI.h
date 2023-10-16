#pragma once
#include "ComponentUI.h"

#include <Engine\Ptr.h>
#include <Engine\CTexture.h>

class TileMapUI :
    public ComponentUI
{
private:
    Ptr<CTexture>       m_AtlasTex;
    Vec2                m_vTileCount;
    Vec2                m_vSlice;

public:
    virtual void update() override;
    virtual void render_update() override;

private:
    void SetTexture(DWORD_PTR _strTexKey);
    void SaveTilemap();
    void SaveGameObject(CGameObject* _Object, FILE* _File);

public:
    TileMapUI();
    ~TileMapUI();
};

