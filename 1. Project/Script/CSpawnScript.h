#pragma once
#include <Engine\CScript.h>
#include "CRoomScript.h"

class CSpawnScript :
    public CScript
{
private:
    float m_fAcctime;
    CGameObject* m_pPlayer;
    CGameObject* m_pRoom;
    MonsterInfo m_sInfo;

public:
    void setAll(CGameObject* _player, CGameObject* _Room, MonsterInfo _info) { m_pPlayer = _player; m_pRoom = _Room; m_sInfo = _info; }
public:
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CSpawnScript);

public:
    CSpawnScript();
    ~CSpawnScript();
};

