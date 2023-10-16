#pragma once
#include <Engine\CScript.h>
#include "CMonsterArm.h"

enum class MONSTERGUN_STATE
{
    IDLE,
    SHOOT,
};

class CMonsterGun :
    public CScript
{
    Monster_type     m_type;
    float            m_fAcctime;
    int              m_iMotion;
    MONSTERGUN_STATE m_eState;

public:
    void setType(Monster_type _type) { m_type = _type; }
    float getPlayerDegree() { return GetOwner()->GetParent()->GetScript<CMonsterArm>()->getPlayerDegree(); }
    void Fire() { m_eState = MONSTERGUN_STATE::SHOOT; }

public:
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CMonsterGun);

public:
    CMonsterGun();
    ~CMonsterGun();
};

