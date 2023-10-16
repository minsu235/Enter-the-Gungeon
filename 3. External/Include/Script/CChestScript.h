#pragma once
#include <Engine\CScript.h>

class CChestScript :
    public CScript
{
private:
    bool        m_bOpened;
    ItemCode    m_eItem;
    bool        m_first;

public:
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CChestScript);

public:
    CChestScript(ItemCode _item = ItemCode::shotgun);
    ~CChestScript();
};

