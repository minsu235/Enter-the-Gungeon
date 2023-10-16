#pragma once
#include <Engine\CScript.h>
#include <Engine/CFontMgr.h>

class CUIScript :
    public CScript
{
private:
    UINT    m_iHP;
    UINT    m_iMaxHP;
    UINT    m_iBlankCount;
    UINT    m_iMaxMagazineBullet;
    UINT    m_iCurrentMagazineBullet;
    UINT    m_iGold;
    UINT    m_iBossHP;

    vector<CGameObject*> m_vecHP;
    vector<CGameObject*> m_vecBlank;
    vector<CGameObject*> m_vecMagazineBullet;
    vector<CGameObject*> m_vecBossHP;
    CGameObject* m_pGun;
    CGameObject* m_pPlayer;

    bool m_bHP;
    bool m_bBlank;
    bool m_bMagazine;
    bool m_bBossHP;

public:
    void setHP(UINT _num) { m_iHP = _num; m_bHP = true; }
    void setBlank(UINT _num) { m_iBlankCount = _num; m_bBlank = true; }
    void setMagazine(UINT _currentnum, UINT _maxnum) { m_iCurrentMagazineBullet = _currentnum; m_iMaxMagazineBullet = _maxnum; m_bMagazine = true; }
    void setCurrentGun(CGameObject* _Gun) { if (m_pGun != nullptr) m_pGun->Destroy(); m_pGun = _Gun; }
    void setPlayer(CGameObject* _player) { m_pPlayer = _player; }
    CGameObject* getCurrentGun() { return m_pGun; }
    void setBossHP(UINT _num) { m_iBossHP = _num; m_bBossHP = true; }
    void setGold(UINT _gold) { m_iGold = _gold; CFontMgr::GetInst()->m_iGold = _gold; }
    void setMaxbullet(UINT _maxbullet, UINT _curbullet) { CFontMgr::GetInst()->m_iMaxBullet = _maxbullet; CFontMgr::GetInst()->m_iBullet = _curbullet; }
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CUIScript);

public:
    CUIScript();
    ~CUIScript();
};

