#pragma once
#include <Engine\CScript.h>

class CMonsterBulletScript :
	public CScript
{
private:
    UINT                m_iDMG;
    Vec3                m_vDIR;
    float               m_fSpeed;
    CGameObject*        m_pPlayer;
    float               m_fAcctime;
    bool                m_bFire;
    float               m_fWaitingtime;

public:
    void setPlayer(CGameObject* _player) { m_pPlayer = _player; }
    void Fire() { if (this == nullptr) return; m_bFire = true; }
    bool beFired() { return m_bFire; }
    void setWaiting(float _time) { m_fWaitingtime = _time; }
    void setDIR(Vec3 _dir) { m_vDIR = _dir; }
    void setSpeed(float _speed) { m_fSpeed = _speed; }

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void reset() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;
public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CMonsterBulletScript);

public:
    CMonsterBulletScript();
    ~CMonsterBulletScript();
};

