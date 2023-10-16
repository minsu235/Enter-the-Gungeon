#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(1000)
	, m_iAliveCount(0)
	, m_vStartScale(Vec4(50.f, 50.f, 1.f, 0.f))
	, m_vEndScale(Vec4(10.f, 10.f, 1.f, 0.f))
	, m_vStartColor(Vec4(0.24f, 0.28f, 0.8f, 1.f))
	, m_vEndColor(Vec4(0.78f, 0.74f, 0.9f, 1.f))
	, m_vMinMaxSpeed(Vec2(100.f, 300.f))
	, m_vMinMaxLifeTime(Vec2(0.2f, 1.f))
	, m_fSpawnRange(200.f)
	, m_Frequency(5.f)
	, m_fAccTime(0.f)
	, m_ParticleBuffer(nullptr)
	, m_ParticleShare(nullptr)
	, m_WorldSpawn(1)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	//GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"SmokeParticle"));
	//GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"CartoonSmoke"));	
	GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Bubbles50px"));


	// ����ȭ���� ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _other)
	: CRenderComponent(_other)
	, m_iMaxCount(_other.m_iMaxCount)
	, m_iAliveCount(0)
	, m_vStartScale(_other.m_vStartScale)
	, m_vEndScale(_other.m_vEndScale)
	, m_vStartColor(_other.m_vStartColor)
	, m_vEndColor(_other.m_vEndColor)
	, m_vMinMaxSpeed(_other.m_vMinMaxSpeed)
	, m_vMinMaxLifeTime(_other.m_vMinMaxLifeTime)
	, m_fSpawnRange(_other.m_fSpawnRange)
	, m_Frequency(_other.m_Frequency)
	, m_fAccTime(_other.m_fAccTime)
	, m_WorldSpawn(_other.m_WorldSpawn)
	, m_ParticleBuffer(nullptr)
	, m_ParticleShare(nullptr)
	, m_UpdateCS(_other.m_UpdateCS)
	, m_SpawnDir(_other.m_SpawnDir)
	, m_SpawnDirRange(_other.m_SpawnDirRange)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	//SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_ParticleShare)
		delete m_ParticleShare;
}

void CParticleSystem::finaltick()
{
	/*Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += 500.f * DT;
	Transform()->SetRelativePos(vPos);*/


	// ��ƼŬ ���� �ð�
	float fAliveTime = 1.f / m_Frequency;

	// �����ð�
	m_fAccTime += DT;

	// �����ð��� ��ƼŬ ���� �ð��� �Ѿ��
	if (fAliveTime < m_fAccTime)
	{
		float f = (m_fAccTime / fAliveTime);
		UINT iAliveCount = (UINT)floorf(f);
		m_fAccTime -= fAliveTime * iAliveCount;

		tParticleShare share = { iAliveCount, };
		//tParticleShare share = { 10, };
		m_ParticleShare->SetData(&share, 1);
	}
	else
	{
		tParticleShare share = {};
		m_ParticleShare->SetData(&share, 1);
	}

	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetParticleShareData(m_ParticleShare);
	m_UpdateCS->SetWorldSpawn(m_WorldSpawn);
	m_UpdateCS->SetObjectWorldPos(Transform()->GetWorldPos());
	m_UpdateCS->SetSpawnRange(m_fSpawnRange);
	m_UpdateCS->SetMinMaxSpeed(m_vMinMaxSpeed);
	m_UpdateCS->SetMinMaxLifeTime(m_vMinMaxLifeTime);
	m_UpdateCS->SetSpawnDir(m_SpawnDir);
	m_UpdateCS->SetSpawnDirRange(m_SpawnDirRange);



	m_UpdateCS->Execute();
}

void CParticleSystem::render()
{
	// ��ġ���� ������Ʈ
	Transform()->UpdateData();
	m_ParticleBuffer->UpdateData(16, PIPELINE_STAGE::VS | PIPELINE_STAGE::GS | PIPELINE_STAGE::PS);

	// ���� ������Ʈ
	GetCurMaterial()->SetScalarParam(INT_1, &m_WorldSpawn);
	GetCurMaterial()->SetScalarParam(VEC4_0, &m_vStartScale);
	GetCurMaterial()->SetScalarParam(VEC4_1, &m_vEndScale);
	GetCurMaterial()->SetScalarParam(VEC4_2, &m_vStartColor);
	GetCurMaterial()->SetScalarParam(VEC4_3, &m_vEndColor);
	GetCurMaterial()->UpdateData();

	// ������
	GetMesh()->render_particle(m_iMaxCount);

	// Ŭ����
	m_ParticleBuffer->Clear();
}

void CParticleSystem::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	// ��ǻƮ ���̴� ���� ����
	bool bCS = !!m_UpdateCS.Get();
	fwrite(&bCS, sizeof(bool), 1, _File);

	if (bCS)
	{
		SaveWStringToFile(m_UpdateCS->GetKey(), _File);
	}	

	// ��ƼŬ �ɼ�
	fwrite(&m_iMaxCount, sizeof(UINT), 1, _File);
	fwrite(&m_iAliveCount, sizeof(UINT), 1, _File);
	fwrite(&m_vStartScale, sizeof(Vec4), 1, _File);
	fwrite(&m_vEndScale, sizeof(Vec4), 1, _File);
	fwrite(&m_vStartColor, sizeof(Vec4), 1, _File);
	fwrite(&m_vEndColor, sizeof(Vec4), 1, _File);
	fwrite(&m_vMinMaxSpeed, sizeof(Vec2), 1, _File);
	fwrite(&m_vMinMaxLifeTime, sizeof(Vec2), 1, _File);
	fwrite(&m_fSpawnRange, sizeof(float), 1, _File);
	fwrite(&m_Frequency, sizeof(float), 1, _File);
	fwrite(&m_fAccTime, sizeof(float), 1, _File);
	fwrite(&m_WorldSpawn, sizeof(int), 1, _File);
	fwrite(&m_SpawnDir, sizeof(float), 1, _File);
	fwrite(&m_SpawnDirRange, sizeof(float), 1, _File);
}

void CParticleSystem::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	// ��ǻƮ���̴� ����
	bool bCS = false;
	fread(&bCS, sizeof(bool), 1, _File);

	if (bCS)
	{
		wstring strKey;
		LoadWStringFromFile(strKey, _File);
		m_UpdateCS = dynamic_cast<CParticleUpdateShader*>(CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get());
	}

	// ��ƼŬ �ɼ�
	fread(&m_iMaxCount, sizeof(UINT), 1, _File);
	fread(&m_iAliveCount, sizeof(UINT), 1, _File);
	fread(&m_vStartScale, sizeof(Vec4), 1, _File);
	fread(&m_vEndScale, sizeof(Vec4), 1, _File);
	fread(&m_vStartColor, sizeof(Vec4), 1, _File);
	fread(&m_vEndColor, sizeof(Vec4), 1, _File);
	fread(&m_vMinMaxSpeed, sizeof(Vec2), 1, _File);
	fread(&m_vMinMaxLifeTime, sizeof(Vec2), 1, _File);
	fread(&m_fSpawnRange, sizeof(float), 1, _File);
	fread(&m_Frequency, sizeof(float), 1, _File);
	fread(&m_fAccTime, sizeof(float), 1, _File);
	fread(&m_WorldSpawn, sizeof(int), 1, _File);
	fread(&m_SpawnDir, sizeof(float), 1, _File);
	fread(&m_SpawnDirRange, sizeof(float), 1, _File);
}

