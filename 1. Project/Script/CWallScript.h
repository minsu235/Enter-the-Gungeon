#pragma once
#include <Engine\CScript.h>
class CWallScript :
    public CScript
{
public:
    virtual void tick() override;

    virtual void Overlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CWallScript);

public:
    CWallScript();
    ~CWallScript();
};

