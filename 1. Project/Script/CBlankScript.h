#pragma once
#include <Engine\CScript.h>
class CBlankScript :
    public CScript
{
private:
    float m_fAcctime;

public:
    virtual void begin() override;
    virtual void tick() override;
    
    virtual void Overlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CBlankScript);

public:
    CBlankScript();
    ~CBlankScript();
};

