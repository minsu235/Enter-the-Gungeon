#pragma once
#include <Engine\CScript.h>

enum class Bookllet_STATE
{
    IDLE,
    CHARGED,
    ATTACK,
    DEATH,
};

class CBooklletScript :
    public CScript
{
private:
    int                     m_iHP;
    Bookllet_STATE          m_eCurrentState;
    CGameObject*            m_pPlayer;
    CGameObject*            m_pRoom;
    float                   m_fAcctime;
    float                   m_fSpeed;
    int                     m_iAttackMotion;
    int                     m_iMotion;
    vector<CGameObject*>    m_vecBullet;

public:
    void setPlayer(CGameObject* _player) { m_pPlayer = _player; }
    void setRoom(CGameObject* _room) { m_pRoom = _room; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CBooklletScript);

public:
    CBooklletScript();
    ~CBooklletScript();
};

