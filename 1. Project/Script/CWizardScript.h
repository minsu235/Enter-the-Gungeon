#pragma once
#include <Engine\CScript.h>

enum class Wizard_STATE
{
    IDLE,
    ATTACK,
    TELEPORT,
    FALL,
    HIT,
    DEATH,
};

class CWizardScript :
    public CScript
{
private:
    int                     m_iHP;
    Wizard_STATE            m_eCurrentState;
    CGameObject*            m_pPlayer;
    CGameObject*            m_pRoom;
    float                   m_fAcctime;
    float                   m_fHittime;
    int                     m_iAttackMotion;
    int                     m_iMotion;
    bool                    m_bInvi;

public:
    void setPlayer(CGameObject* _player) { m_pPlayer = _player; }
    void setRoom(CGameObject* _room) { m_pRoom = _room; }

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

    CLONE(CWizardScript);

public:
    CWizardScript();
    ~CWizardScript();
};

