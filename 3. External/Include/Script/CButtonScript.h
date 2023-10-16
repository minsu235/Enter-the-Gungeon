#pragma once
#include <Engine\CScript.h>
class CButtonScript :
    public CScript
{
private:
    bool m_bMouseOn;

public:
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CButtonScript);

public:
    CButtonScript();
    ~CButtonScript();
};

