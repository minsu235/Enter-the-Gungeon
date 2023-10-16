#pragma once
#include <Engine\CScript.h>

enum class Gunnut_STATE
{
    IDLE,
    ATTACK_B,
    ATTACK_F,
    ATTACK_BL,
    ATTACK_BR,
    ATTACK_FL,
    ATTACK_FR,
    ATTACK_L,
    ATTACK_R,
    WALK,
    DEATH,
};

class CGunnutScript :
    public CScript
{
private:
    int                     m_iHP;
    Gunnut_STATE            m_eCurrentState;
    CGameObject*            m_pPlayer;
    CGameObject*            m_pRoom;
    float                   m_fSpeed;
    float                   m_fAcctime;
    float                   m_fWalktime;
    int                     m_iMotion;

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

    CLONE(CGunnutScript);

public:
    CGunnutScript();
    ~CGunnutScript();
};

