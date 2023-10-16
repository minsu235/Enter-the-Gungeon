#pragma once
#include <Engine\CScript.h>
class CWingScript :
    public CScript
{
private:
    CGameObject* m_pPlayer;

public:
    virtual void tick() override;
    void setPlayer(CGameObject* _player) { m_pPlayer = _player; }
public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CWingScript);

public:
    CWingScript();
    ~CWingScript();
};

