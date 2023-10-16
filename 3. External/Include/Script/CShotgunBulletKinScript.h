#pragma once
#include <Engine\CScript.h>

enum class ShotgunBulletKin_STATE
{
    IDLE,
    FALL,
    WALK,
    DEATH,
};

class CShotgunBulletKinScript :
    public CScript
{
private:
    int                     m_iHP;
    ShotgunBulletKin_STATE  m_eCurrentState;
    CGameObject*            m_pPlayer;
    CGameObject*            m_pRoom;
    float                   m_fSpeed;
    float                   m_fAcctime;
    float                   m_fWalktime;
    int                     m_iMotion;
    float                   m_fHittime;

public:
    void setPlayer(CGameObject* _player) { m_pPlayer = _player; }
    void setRoom(CGameObject* _room) { m_pRoom = _room; }
    CGameObject* getPlayer() { return m_pPlayer; }
    ShotgunBulletKin_STATE getState() { return m_eCurrentState; }
private:
    float getPlayerDegree();

public:
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CShotgunBulletKinScript);

public:
    CShotgunBulletKinScript();
    ~CShotgunBulletKinScript();
};

