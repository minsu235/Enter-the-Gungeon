#pragma once
#include <Engine\CScript.h>

class CPlayerArmScript :
    public CScript
{
private:
    Vec3    m_offset;
    Vec3    m_armpos;
    bool    right;

public:
    void setOffset(Vec3 _offset) { m_offset = _offset; }
    void setArmpos(Vec3 _pos) { m_armpos = _pos; }
private:
    float getMouseDIR();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CPlayerArmScript);

public:
    CPlayerArmScript();
    ~CPlayerArmScript();
};

