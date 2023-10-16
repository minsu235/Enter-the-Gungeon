#pragma once
#include <Engine\CScript.h>

enum class Monster_type
{
    shotgun,
    magnum,
};

class CMonsterArm :
    public CScript
{
private:
    Vec3            m_offset;
    Vec3            m_armpos;
    bool            right;
    Monster_type    m_type;

public:
    void setOffset(Vec3 _offset) { m_offset = _offset; }
    void setArmpos(Vec3 _pos) { m_armpos = _pos; }
    void setType(Monster_type _type) { m_type = _type; }
    float getPlayerDegree();

public:
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CMonsterArm);

public:
    CMonsterArm();
    ~CMonsterArm();
};

