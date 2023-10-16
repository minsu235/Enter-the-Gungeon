#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    UINT                        m_iMaxCount;
    UINT                        m_iAliveCount;

    Vec4                        m_vStartScale;
    Vec4                        m_vEndScale;

    Vec4                        m_vStartColor;
    Vec4                        m_vEndColor;

    Vec2                        m_vMinMaxSpeed;
    Vec2                        m_vMinMaxLifeTime;

    float                       m_fSpawnRange;

    float                       m_Frequency;    // 초당 파티클 생성 수
    float                       m_fAccTime;     // 시간 누적
    float                       m_SpawnDir;
    float                       m_SpawnDirRange;

    int                         m_WorldSpawn;

    CStructuredBuffer*          m_ParticleBuffer;
    CStructuredBuffer*          m_ParticleShare;
    Ptr<CParticleUpdateShader>  m_UpdateCS;


public:
    void SetStartScale(Vec4 _in) { m_vStartScale = _in; }
    Vec4 GetStartScale() { return m_vStartScale; }
    void SetEndScale(Vec4 _in) { m_vEndScale = _in; }
    Vec4 GetEndScale() { return m_vEndScale; }

    void SetStartColor(Vec4 _in) { m_vStartColor = _in; }
    Vec4 GetStartColor() { return m_vStartColor; }
    void SetEndColor(Vec4 _in) { m_vEndColor = _in; }
    Vec4 GetEndColor() { return m_vEndColor; }

    void SetMinMaxSpeed(Vec2 _in) { m_vMinMaxSpeed = _in; }
    Vec2 GetMinMaxSpeed() { return m_vMinMaxSpeed; }
    void SetMinMaxLifeTime(Vec2 _in) { m_vMinMaxLifeTime = _in; }
    Vec2 GetMinMaxLifeTime() { return m_vMinMaxLifeTime; }

    void SetSpawnRange(float _in) { m_fSpawnRange = _in; }
    float GetSpawnRange() { return m_fSpawnRange; }
    void SetFrequency(float _in) { m_Frequency = _in; }
    float GetFrequency() { return m_Frequency; }
    void SetSpawnDir(float _in) { m_SpawnDir = _in; }
    float GetSpawnDir() { return m_SpawnDir; }
    void SetSpawnDirRange(float _in) { m_SpawnDirRange = _in; }
    float GetSpawnDirRange() { return m_SpawnDirRange; }


    void SetWorldSpawn(bool _bSet) { m_WorldSpawn = (int)_bSet; }
    bool GetWorldSpawn() { return (bool)m_WorldSpawn; }

    void SetUpdateCS(CParticleUpdateShader* _shader) { m_UpdateCS = _shader; }

public:
    virtual void finaltick() override;
    virtual void render() override;


public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _other);
    ~CParticleSystem();
};

