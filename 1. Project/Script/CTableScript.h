#pragma once
#include <Engine\CScript.h>

enum class Table_State
{
    L,
    R,
    F,
    B,
    IDLE,
};

class CTableScript :
    public CScript
{
    Table_State         m_eState;
    int                 m_iHP;
    bool                m_bFirst;

public:
    virtual void tick() override;

    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CTableScript);

public:
    CTableScript();
    ~CTableScript();

    friend class CTableScript;
};

