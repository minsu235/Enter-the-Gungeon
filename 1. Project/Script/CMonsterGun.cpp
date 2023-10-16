#include "pch.h"
#include "CMonsterGun.h"

#include "CMonsterBulletScript.h"

CMonsterGun::CMonsterGun()
	: CScript(MONSTERGUN)
	, m_type(Monster_type::magnum)
	, m_fAcctime(0.f)
	, m_iMotion(0)
	, m_eState(MONSTERGUN_STATE::IDLE)
{
}

CMonsterGun::~CMonsterGun()
{
}

void CMonsterGun::tick()
{
	if (m_eState == MONSTERGUN_STATE::SHOOT)
	{
		if (m_type == Monster_type::shotgun)
		{
			Vec3 vPos = Transform()->GetWorldPos();
			float PlayerDegree = getPlayerDegree();
			for (int i = 0; i < 5; ++i)
			{
				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((PlayerDegree - 20.f + 10 * i) * 3.141592f / 180.f), sinf((PlayerDegree - 20.f + 10.f * i) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);
			}
			m_eState = MONSTERGUN_STATE::IDLE;
		}
		else if (m_type == Monster_type::magnum)
		{
			m_fAcctime += DT;
			if (m_fAcctime > 0 && m_iMotion == 0)
			{
				++m_iMotion;
				Vec3 vPos = Transform()->GetWorldPos();
				float PlayerDegree = getPlayerDegree();
				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setSpeed(500.f);
				float ran = cosf((float)rand());
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((PlayerDegree + 10 * ran) * 3.141592f / 180.f), sinf((PlayerDegree + 10 * ran) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\magnum_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
				pSound->Play(1, 0.6f, true);
			}
			else if (m_fAcctime > 0.5f && m_iMotion == 1)
			{
				++m_iMotion;
				Vec3 vPos = Transform()->GetWorldPos();
				float PlayerDegree = getPlayerDegree();
				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setSpeed(500.f);
				float ran = cosf((float)rand()*5);
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((PlayerDegree + 10 * ran) * 3.141592f / 180.f), sinf((PlayerDegree + 10 * ran) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\magnum_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
				pSound->Play(1, 0.6f, true);
			}
			else if (m_fAcctime > 1.f && m_iMotion == 2)
			{
				Vec3 vPos = Transform()->GetWorldPos();
				float PlayerDegree = getPlayerDegree();
				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setSpeed(500.f);
				float ran = cosf((float)rand()*2);
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((PlayerDegree + 10 * ran) * 3.141592f / 180.f), sinf((PlayerDegree + 10 * ran) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\magnum_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
				pSound->Play(1, 0.6f, true);
				m_iMotion = 0;
				m_fAcctime = 0.f;
				m_eState = MONSTERGUN_STATE::IDLE;
			}
		}
	}
}

void CMonsterGun::SaveToFile(FILE* _File)
{
}

void CMonsterGun::LoadFromFile(FILE* _File)
{
}


