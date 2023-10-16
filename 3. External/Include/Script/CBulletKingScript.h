#pragma once
#include <Engine\CScript.h>

enum class BulletKing_STATE
{
    MOVE,
    STOP,
    DEATH,
};
class CBulletKingScript :
    public CScript
{
private:
    int                     m_iHP;
    BulletKing_STATE        m_eCurrentState;
    CGameObject*            m_pPlayer;
    float                   m_fSpeed;
    float                   m_fAcctime;
    int                     m_iMotion;
    int                     m_iAct;
    int                     m_iDIR;
    int                     m_b3attack;

public:
    void setPlayer(CGameObject* _player) { m_pPlayer = _player; }

private:
    float getPlayerDegree();
    void Attack_spread();
    void Attack_3shoot_r();
    void Attack_3shoot_l();
    void Attack_3shoot_fr();
    void Attack_3shoot_fl();
    void Attack_pound();
    void Attack_spin();
public:
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CBulletKingScript);

public:
    CBulletKingScript();
    ~CBulletKingScript();
};

