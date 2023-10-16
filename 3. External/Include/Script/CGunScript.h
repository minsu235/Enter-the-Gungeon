#pragma once
#include <Engine\CScript.h>

enum class GUN_STATE
{
    IDLE,
    RELOAD,
    SHOOT,
    DEATH,
};

class CGunScript :
    public CScript
{
private:
    UINT            m_MaximumBullet;
    UINT            m_MagazineBullet;
    float           m_fReloadtime;
    float           m_fBulletSpread;
    float           m_fFireInterval;

    UINT            m_iCurrentMagazine;
    UINT            m_iCurrentBullet;
    float           m_fAcctime;
    float           m_fCharge;
    float           m_fChargedtime;

    GUN_STATE       m_eCurrentState;
    GUN             m_gun;
    Ptr<CPrefab>    m_bullet;
    CGameObject*    m_UI;

    bool            m_act;

    CGameObject*    m_Reload_bar;
    CGameObject*    m_Reload;

public:
    void setGUN(GUN _gun) { m_gun = _gun; }
    void setUI(CGameObject* _ui) { m_UI = _ui; }
    void setMaximumBullet(UINT _num) { m_MaximumBullet = _num; m_iCurrentBullet = _num; }
    void setMagazineBullet(UINT _num) { m_MagazineBullet = _num; m_iCurrentMagazine = _num; }
    void setFireInterval(float _time) { m_fFireInterval = _time; }
    void setReloadtime(float _time) { m_fReloadtime = _time; }
    void setBulletSpread(float _degree) { m_fBulletSpread = _degree; }
    void setBullet(Ptr<CPrefab> _bullet) { m_bullet = _bullet; }
    void setAct();
    void setDeact();
    void setBoolact(bool _act) { m_act = _act; }

private:
    float getMouseDIR();
    void Reload(bool _delete = false);
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CGunScript);

public:
    CGunScript();
    ~CGunScript();
};

