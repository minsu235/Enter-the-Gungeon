#pragma once
#include <Engine\CScript.h>

class CGameCameraScript :
    public CScript
{
private:
    CGameObject* m_player;
    float        m_fAcctime;
    bool         m_bCamerashake;
    int          m_iAct;
    bool         m_bBoss;
    bool         m_bStart;
    float        m_fSpeed;

public:
    void setPlayer(CGameObject* _player) { m_player = _player; }
    void camerashake() { m_bCamerashake = true; m_iAct = 0; m_fAcctime = 0.f; }
    void camerashake_bossdoor() { if(!m_bCamerashake) m_iAct = 10; m_bCamerashake = true; }
    void BossCameraMove(Vec3 _bossPos);

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Move();

    CLONE(CGameCameraScript);
public:
    CGameCameraScript();
    ~CGameCameraScript();
};

