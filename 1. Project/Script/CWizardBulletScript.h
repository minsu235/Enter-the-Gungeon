#pragma once
#include <Engine\CScript.h>

class CWizardBulletScript :
    public CScript
{
private:
    float m_fSpeed;
    float m_fAcctime;
    Vec3 m_vDIR;

public:
    void setDIR(Vec3 _dir) { m_vDIR = _dir; }
public:
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CWizardBulletScript);

public:
    CWizardBulletScript();
    ~CWizardBulletScript();

};

