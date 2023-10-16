#include "pch.h"
#include "CSpawnScript.h"

#include "CBooklletScript.h"
#include "CBulletKingScript.h"
#include "CBulletKinScript.h"
#include "CGunnutScript.h"
#include "CShotgunBulletKinScript.h"
#include "CWizardScript.h"

CSpawnScript::CSpawnScript()
	: CScript(SPAWNSCRIPT)
	, m_fAcctime(0.f)
	, m_pPlayer(nullptr)
	, m_pRoom(nullptr)
	, m_sInfo{}
{
}

CSpawnScript::~CSpawnScript()
{
}

void CSpawnScript::tick()
{
	if (m_fAcctime >= 0.f)
	{
		m_fAcctime += DT;
		if (m_fAcctime >= 1.0625f)
		{
			m_fAcctime = -1.f;
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\spawn_appear_01.wav");
			pSound->Play(1, 0.6f, true);
			switch (m_sInfo.m_eMonster)
			{
			case Monster::Bookllet:
			{
				Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Bookllet");
				CGameObject* monster = prefab->Instantiate();
				monster->GetScript<CBooklletScript>()->setPlayer(m_pPlayer);
				monster->GetScript<CBooklletScript>()->setRoom(m_pRoom);
				Instantiate(monster, m_sInfo.m_vPos, 3);
			}
			break;
			case Monster::BulletKin:
			{
				Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"BulletKin");
				CGameObject* monster = prefab->Instantiate();
				monster->GetScript<CBulletKinScript>()->setPlayer(m_pPlayer);
				monster->GetScript<CBulletKinScript>()->setRoom(m_pRoom);
				Instantiate(monster, m_sInfo.m_vPos, 3);
			}
			break;
			case Monster::Gunnut:
			{
				Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Gunnut");
				CGameObject* monster = prefab->Instantiate();
				monster->GetScript<CGunnutScript>()->setPlayer(m_pPlayer);
				monster->GetScript<CGunnutScript>()->setRoom(m_pRoom);
				Instantiate(monster, m_sInfo.m_vPos, 3);
			}
			break;
			case Monster::ShotgunKinRed:
			{
				Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"ShotgunKinRed");
				CGameObject* monster = prefab->Instantiate();
				monster->GetScript<CShotgunBulletKinScript>()->setPlayer(m_pPlayer);
				monster->GetScript<CShotgunBulletKinScript>()->setRoom(m_pRoom);
				Instantiate(monster, m_sInfo.m_vPos, 3);
			}
			break;
			case Monster::ShotgunKinBlue:
			{
				Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"ShotgunKinBlue");
				CGameObject* monster = prefab->Instantiate();
				monster->GetScript<CShotgunBulletKinScript>()->setPlayer(m_pPlayer);
				monster->GetScript<CShotgunBulletKinScript>()->setRoom(m_pRoom);
				Instantiate(monster, m_sInfo.m_vPos, 3);
			}
			break;
			case Monster::Wizard:
			{
				Ptr<CPrefab> prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"Wizard");
				CGameObject* monster = prefab->Instantiate();
				monster->GetScript<CWizardScript>()->setPlayer(m_pPlayer);
				monster->GetScript<CWizardScript>()->setRoom(m_pRoom);
				Instantiate(monster, m_sInfo.m_vPos, 3);
			}
			break;
			}
		}
	}
}

void CSpawnScript::SaveToFile(FILE* _File)
{
}

void CSpawnScript::LoadFromFile(FILE* _File)
{
}