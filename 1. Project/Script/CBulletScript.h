#pragma once
#include <Engine\CScript.h>

class CBulletScript :
    public CScript
{
private:
    UINT m_iDMG;
    float m_fDIR;
    float m_fSpeed;
    GUN m_gun;
    float m_fAcctime;
    float m_fDuration;

public:
    void setDIR(float _degree) { m_fDIR = _degree; }
    void setGUN(GUN _gun) { m_gun = _gun; }
    void setSpeed(float _speed) { m_fSpeed = _speed; }
    void setDMG(UINT _DMG) { m_iDMG = _DMG; }
    UINT getDMG() 
    { 
        Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\general_hurt_01.wav"); 
        pSound->Play(1, 0.4f, true); 
        if (m_iDMG == 45)
        {
            CGameObject* bullet_impact = CResMgr::GetInst()->FindRes<CPrefab>(L"megabuster_impact")->Instantiate();
            bullet_impact->Animator2D()->Play(L"megabuster_impact", false);
            Instantiate(bullet_impact, Transform()->GetRelativePos(), 0);
        }
        else
        {
            CGameObject* bullet_impact = CResMgr::GetInst()->FindRes<CPrefab>(L"bullet_impact")->Instantiate();
            bullet_impact->Animator2D()->Play(L"bullet_impact", false);
            Instantiate(bullet_impact, Transform()->GetRelativePos(), 0);
        }
        return m_iDMG; 
    }
    float getDIR() { return m_fDIR; }
    void setDuration(float _time) { m_fDuration = _time; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;
public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CBulletScript);

public:
    CBulletScript();
    ~CBulletScript();
};

