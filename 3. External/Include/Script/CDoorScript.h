#pragma once
#include <Engine\CScript.h>
class CDoorScript :
    public CScript
{
private:
    bool        m_first;
    bool        m_bBossdoor;
    float       m_fAcctime;
    bool        m_bOpen;
public:
    void setBossdoor() { m_bBossdoor = true; }
public:
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CDoorScript);

public:
    CDoorScript();
    ~CDoorScript();
};

