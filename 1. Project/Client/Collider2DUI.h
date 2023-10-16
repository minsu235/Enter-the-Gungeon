#pragma once
#include "ComponentUI.h"
class Collider2DUI :
    public ComponentUI
{
private:
    Vec2            m_vOffsetPos;
    Vec2            m_vScale;
    Vec3            m_vRot;

    COLLIDER2D_TYPE m_eType;
    bool            m_bIgnorObjectScale;
    int             m_coll_item_current;

public:
    virtual void update() override;
    virtual void render_update() override;


private:
    void SetMesh(DWORD_PTR _strMeshKey);
    void SetMaterial(DWORD_PTR _strMaterialKey);


public:
    Collider2DUI();
    ~Collider2DUI();
};

