#pragma once
#include "ComponentUI.h"
class ParticleSystemUI :
    public ComponentUI
{
private:
    Vec4        m_vStartScale;
    Vec4        m_vEndScale;
    Vec4        m_vStartColor;
    Vec4        m_vEndColor;

    float       m_vMinMaxSpeed[2];
    float       m_vMinMaxLifeTime[2];

    int         m_fSpawnRange;

    float       m_Frequency;
    float       m_SpawnDir;
    float       m_SpawnDirRange;

    int         m_WorldSpawn;
public:
    virtual void update() override;
    virtual void render_update() override;

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

