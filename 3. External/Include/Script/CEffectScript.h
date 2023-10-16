#pragma once
#include <Engine\CScript.h>
class CEffectScript :
    public CScript
{
private:
    float m_fAcctime;
    float m_fEffecttime;

public:
    void setEffecttime(float _time) { m_fEffecttime = _time; }
public:
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CEffectScript);

public:
    CEffectScript();
    ~CEffectScript();
};

