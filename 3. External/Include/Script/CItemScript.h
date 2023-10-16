#pragma once
#include <Engine\CScript.h>

class CItemScript :
    public CScript
{
private:
    ItemCode    m_eItem;

public:
    virtual void tick() override {};

    virtual void Overlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CItemScript);

public:
    CItemScript(ItemCode _item = ItemCode::shotgun);
    ~CItemScript();
};

