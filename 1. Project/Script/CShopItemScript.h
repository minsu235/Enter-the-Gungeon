#pragma once
#include <Engine\CScript.h>

class CShopItemScript :
    public CScript
{
private:
    ItemCode        m_eItem;
    UINT            m_iPrice;
    bool            m_bSoldout;
    CGameObject*    m_pTag;

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CShopItemScript);

public:
    CShopItemScript(ItemCode _item = ItemCode::heart);
    ~CShopItemScript();
};

