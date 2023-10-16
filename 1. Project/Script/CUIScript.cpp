#include "pch.h"
#include "CUIScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CResMgr.h>

CUIScript::CUIScript()
	: CScript(UISCRIPT)
	, m_iHP(6)
	, m_iMaxHP(6)
	, m_iBlankCount(2)
	, m_iMaxMagazineBullet(6)
	, m_iCurrentMagazineBullet(6)
	, m_pGun(nullptr)
	, m_bHP(true)
	, m_bBlank(true)
	, m_bMagazine(true)
	, m_bBossHP(false)
	, m_pPlayer(nullptr)
	, m_iGold(0)
	, m_iBossHP(0)
{
}

CUIScript::~CUIScript()
{
}

void CUIScript::begin()
{
}

void CUIScript::tick()
{
	if (m_bHP)
	{
		m_bHP = false;
		vector<CGameObject*>::iterator iter = m_vecHP.begin();
		for (; iter != m_vecHP.end(); ++iter)
		{
			(*iter)->Destroy();
		}
		m_vecHP.clear();

		UINT hp = m_iHP / 2;
		UINT i = 0;
		for (; i < hp; ++i)
		{
			Vec3 vPos = Vec3(-687.f + i * 48.f, 373.f, 1.f);

			CGameObject* Heart = CResMgr::GetInst()->FindRes<CPrefab>(L"heart_full")->Instantiate();
			m_vecHP.push_back(Heart);
			Instantiate(Heart, vPos, 31);
		}

		hp = m_iHP % 2;
		if (hp != 0)
		{
			Vec3 vPos = Vec3(-687.f + i * 48.f, 373.f, 1.f);

			CGameObject* Heart = CResMgr::GetInst()->FindRes<CPrefab>(L"heart_half")->Instantiate();
			m_vecHP.push_back(Heart);
			Instantiate(Heart, vPos, 31);
			++i;
		}

		hp = (m_iMaxHP - m_iHP) / 2;
		for (; i < m_iMaxHP / 2; ++i)
		{
			Vec3 vPos = Vec3(-687.f + i * 48.f, 373.f, 1.f);

			CGameObject* Heart = CResMgr::GetInst()->FindRes<CPrefab>(L"heart_empty")->Instantiate();
			m_vecHP.push_back(Heart);
			Instantiate(Heart, vPos, 31);
		}
	}

	if (m_bBlank)
	{
		m_bBlank = false;
		vector<CGameObject*>::iterator iter = m_vecBlank.begin();
		for (; iter != m_vecBlank.end(); ++iter)
		{
			(*iter)->Destroy();
		}
		m_vecBlank.clear();

		for (UINT i = 0; i < m_iBlankCount; ++i)
		{
			Vec3 vPos = Vec3(-692.f + i * 40.f, 334.f, 1.f);

			CGameObject* Blank = CResMgr::GetInst()->FindRes<CPrefab>(L"blank")->Instantiate();
			m_vecBlank.push_back(Blank);
			Instantiate(Blank, vPos, 31);
		}
	}

	if (m_bMagazine)
	{
		m_bMagazine = false;
		vector<CGameObject*>::iterator iter = m_vecMagazineBullet.begin();
		for (; iter != m_vecMagazineBullet.end(); ++iter)
		{
			(*iter)->Destroy();
		}
		m_vecMagazineBullet.clear();

		Vec3 vPos = Vec3(700.f, -386.f, 1.f);

		CGameObject* bullet = CResMgr::GetInst()->FindRes<CPrefab>(L"bullet_bottomUI")->Instantiate();
		m_vecMagazineBullet.push_back(bullet);
		Instantiate(bullet, vPos, 31);

		UINT i = 0;
		for (; i < m_iCurrentMagazineBullet; ++i)
		{
			vPos = Vec3(700.f, -370.f + i * 16.f, 1.f);

			bullet = CResMgr::GetInst()->FindRes<CPrefab>(L"bulletUI")->Instantiate();
			m_vecMagazineBullet.push_back(bullet);
			Instantiate(bullet, vPos, 31);
		}

		for (; i < m_iMaxMagazineBullet; ++i)
		{
			vPos = Vec3(700.f, -370.f + i * 16.f, 1.f);

			bullet = CResMgr::GetInst()->FindRes<CPrefab>(L"bullet_emptyUI")->Instantiate();
			m_vecMagazineBullet.push_back(bullet);
			Instantiate(bullet, vPos, 31);
		}

		vPos = Vec3(700.f, -370.f + i * 16.f, 1.f);

		bullet = CResMgr::GetInst()->FindRes<CPrefab>(L"bullet_topUI")->Instantiate();
		m_vecMagazineBullet.push_back(bullet);
		Instantiate(bullet, vPos, 31);
	}

	if (m_bBossHP)
	{
		m_bBossHP = false;
		vector<CGameObject*>::iterator iter = m_vecBossHP.begin();
		for (; iter != m_vecBossHP.end(); ++iter)
		{
			(*iter)->Destroy();
		}
		m_vecBossHP.clear();

		if (m_iBossHP != 0)
		{
			CGameObject* HP = CResMgr::GetInst()->FindRes<CPrefab>(L"BossHPBar")->Instantiate();
			m_vecBossHP.push_back(HP);
			Instantiate(HP, Vec3(0.f, -360.f, 2.f), 31);
			HP = CResMgr::GetInst()->FindRes<CPrefab>(L"BossHP")->Instantiate();
			HP->Transform()->SetRelativeScale(Vec3((float)m_iBossHP / 200.f * 498.f, 42.f, 0.f));
			m_vecBossHP.push_back(HP);
			Instantiate(HP, Vec3(-(249.f - (float)m_iBossHP / 200.f * 249.f), -360.f, 1.f), 31);
		}
	}
}

void CUIScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CUIScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}


