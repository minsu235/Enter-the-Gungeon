#pragma once

#include <Engine\CScript.h>
#include "CUIScript.h"

enum class PLAYER_STATE
{
    DEATH,
    DODGE_B,
    DODGE_F,
    DODGE_R,
    DODGE_L,
    DODGE_FR,
    DODGE_FL,
    DODGE_BR,
    DODGE_BL,
    RUN_B,
    RUN_F,
    RUN_R,
    RUN_L,
    RUN_FR,
    RUN_FL,
    RUN_BR,
    RUN_BL,
    SPIN,
    SLIDE_B,
    SLIDE_F,
    SLIDE_R,
    SLIDE_L,
    SLIDE_FR,
    SLIDE_FL,
    SLIDE_BR,
    SLIDE_BL,
    IDLE_B,
    IDLE_F,
    IDLE_R,
    IDLE_L,
    IDLE_FR,
    IDLE_FL,
    IDLE_BR,
    IDLE_BL,
    DODGEFALL_B,
    DODGEFALL_F,
    DODGEFALL_R,
    DODGEFALL_L,
    DODGEFALL_FR,
    DODGEFALL_FL,
    DODGEFALL_BR,
    DODGEFALL_BL,
    FALL_B,
    FALL_F,
    FALL_R,
    FALL_L,
};

enum class MOUSE_DIR
{
    U,
    UR,
    UL,
    D,
    DR,
    DL,
    R,
    L,
};

class CPlayerScript :
    public CScript
{
private:
    float               m_fAccTime;
    float               m_fSpeed;
    PLAYER_STATE        m_eCurrentState;
    UINT                m_iHP;
    UINT                m_iBlankCount;
    CGameObject*        m_UI;
    bool                m_bInvi;
    float               m_fInvitime;
    float               m_fRuntime;
    int                 m_iRunCount;
    bool                m_bFall;
    int                 m_iCurrentGun;

    float               m_fSpread;
    bool                m_bFly;
    int                 m_iGold;
    bool                m_bStop;

public:
    PLAYER_STATE getPlayerState() { return m_eCurrentState; }
    void setUI(CGameObject* _ui) { m_UI = _ui; }
    CGameObject* getUI() { return m_UI; }
    void setSpread(float _num) { m_fSpread = _num; }
    float getSpread() { return m_fSpread; }
    void setFly(bool _bool) { m_bFly = _bool; }
    MOUSE_DIR getDIR();
    bool addGold(int _iGold) { if (m_iGold + _iGold < 0) return false; m_iGold += _iGold; m_UI->GetScript<CUIScript>()->setGold(m_iGold); return true; }
    void addHP(UINT _HP) { m_iHP += _HP; if (m_iHP > 6) m_iHP = 6; m_UI->GetScript<CUIScript>()->setHP(m_iHP); }
    void addBlank() { m_iBlankCount += 1; m_UI->GetScript<CUIScript>()->setBlank(m_iBlankCount); }
    void PlayerStop(bool _bool) { m_bStop = _bool; }
    bool IsStop() { return m_bStop; }
private:
    void playRun();
    void playIdle();
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CPlayerScript);

public:
    CPlayerScript();
    ~CPlayerScript();
};

