#include "pch.h"
#include "CBooklletScript.h"

#include "CMonsterBulletScript.h"
#include "CRoomScript.h"
#include "CBulletScript.h"
#include "CPlayerScript.h"

CBooklletScript::CBooklletScript()
	: CScript(BOOKLLETSCRIPT)
	, m_iHP(25)
	, m_eCurrentState(Bookllet_STATE::IDLE)
	, m_pPlayer(nullptr)
	, m_pRoom(nullptr)
	, m_fAcctime(0.f)
	, m_fSpeed(120.f)
	, m_iAttackMotion(0)
	, m_iMotion(0)
{
}

CBooklletScript::~CBooklletScript()
{
}

void CBooklletScript::begin()
{
}

void CBooklletScript::tick()
{
	if (m_pPlayer == nullptr)
		return;
	if (m_eCurrentState == Bookllet_STATE::IDLE)
	{
		if (m_fAcctime > 5.f)
		{
			m_eCurrentState = Bookllet_STATE::CHARGED;
			Animator2D()->Play(L"CHARGED", true);
			m_fAcctime = 0.f;
		}
		else
		{
			Animator2D()->Play(L"IDLE", true);
			m_fAcctime += DT;
			if (RigidBody()->GetVelocity() == Vec3(0.f, 0.f, 0.f))
			{
				if (m_pPlayer != nullptr)
				{
					Vec3 playerpos = m_pPlayer->Transform()->GetRelativePos();
					playerpos.y -= 30.f;
					playerpos.z = 0.f;
					Vec3 monsterpos = Transform()->GetRelativePos();
					monsterpos.z = 0.f;
					float distance = Vec3::Distance(playerpos, monsterpos);
					if (distance > 100.f)
					{
						Vec2 playerpos = m_pRoom->GetScript<CRoomScript>()->FindPath(GetOwner());

						Vec3 Dirrection = playerpos - monsterpos;
						Dirrection.Normalize();
						Vec3 vPos = Transform()->GetRelativePos();
						vPos.x += Dirrection.x * m_fSpeed * DT;
						vPos.y += Dirrection.y * m_fSpeed * DT;
						Transform()->SetRelativePos(vPos);
					}
				}
			}
		}
	}
	else if (m_eCurrentState == Bookllet_STATE::DEATH)
	{
		if (m_fAcctime == 0.f)
		{
			vector<CGameObject*>::iterator iter = m_vecBullet.begin();
			for (; iter != m_vecBullet.end(); ++iter)
			{
				CGameObject* bullet = *iter;
				bullet->GetScript<CMonsterBulletScript>()->Fire();
			}
			m_vecBullet.clear();
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\general_death_01.wav");
			pSound->Play(1, 0.5f, true);
		}
		m_fAcctime += DT;
		if (m_fAcctime > 0.5f && !GetOwner()->IsDead())
		{
			CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup")->Instantiate();
			impact->Animator2D()->Play(L"dustup", false);
			Instantiate(impact, Transform()->GetRelativePos(), 0);
			Destroy();
			m_pRoom->GetScript<CRoomScript>()->MonsterDie();
			m_pPlayer->GetScript<CPlayerScript>()->addGold(15);
		}
	}
	else if (m_eCurrentState == Bookllet_STATE::CHARGED)
	{
		m_fAcctime += DT;
		if (m_fAcctime > 1.f)
		{
			m_eCurrentState = Bookllet_STATE::ATTACK;
			Animator2D()->Play(L"ATTACK", true);
			m_iAttackMotion = 0;//(int)(DT * 10000) % 3;
			m_fAcctime = 0.f;
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\book_blast_01.wav");
			pSound->Play(1, 0.5f, true);
		}
	}
	else if (m_eCurrentState == Bookllet_STATE::ATTACK)
	{
		m_fAcctime += DT;

		if (m_fAcctime > 2.f)
		{
			m_eCurrentState = Bookllet_STATE::IDLE;
			Animator2D()->Play(L"IDLE", true);
			m_fAcctime = 0.f;
			m_iMotion = 0;
		}
		//0~0.8 총알
		//0.8~1.0
		//1.0 발사
		if (m_iAttackMotion == 0)
		{
			if (m_fAcctime > 1.f && m_iMotion == 21)
			{
				vector<CGameObject*>::iterator iter = m_vecBullet.begin();
				for (; iter != m_vecBullet.end(); ++iter)
				{
					CGameObject* bullet = *iter;
					bullet->GetScript<CMonsterBulletScript>()->Fire();
				}
				m_vecBullet.clear();
				++m_iMotion;
			}
			else if (m_fAcctime > 0.6f && m_iMotion == 20)
			{
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, 60.f, 0.01f);
				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, 1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, 40.f, -0.01f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, 1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, 30.f, -0.02f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, 1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, 20.f, -0.03f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, 1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, 10.f, -0.04f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, 1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, 0.f, -0.045f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, 1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, -10.f, -0.05f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, -1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, -20.f, -0.06f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, -1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, -30.f, -0.07f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, -1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, -40.f, -0.08f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, -1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				vMissilePos = Transform()->GetWorldPos() + Vec3(0.f, -60.f, -0.1f);
				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(0.f, -1.f, 0.f));
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);

				++m_iMotion;
			}
			else if (m_fAcctime > 0.57f && m_iMotion == 19)
			{
				float degree = 72.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.54f && m_iMotion == 18)
			{
				float degree = 54.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.51f && m_iMotion == 17)
			{
				float degree = 36.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.48f && m_iMotion == 16)
			{
				float degree = 18.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.45f && m_iMotion == 15)
			{
				float degree = 0.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.42f && m_iMotion == 14)
			{
				float degree = 342.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.39f && m_iMotion == 13)
			{
				float degree = 324.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.36f && m_iMotion == 12)
			{
				float degree = 306.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.33f && m_iMotion == 11)
			{
				float degree = 288.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.3f && m_iMotion == 10)
			{
				float degree = 270.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), -0.09f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.27f && m_iMotion == 9)
			{
				float degree = 252.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.24f && m_iMotion == 8)
			{
				float degree = 234.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.21f && m_iMotion == 7)
			{
				float degree = 216.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.18f && m_iMotion == 6)
			{
				float degree = 198.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.15f && m_iMotion == 5)
			{
				float degree = 180.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.12f && m_iMotion == 4)
			{
				float degree = 162.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.09f && m_iMotion == 3)
			{
				float degree = 144.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.06f && m_iMotion == 2)
			{
				float degree = 126.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.03f && m_iMotion == 1)
			{
				float degree = 108.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
			else if (m_fAcctime > 0.f && m_iMotion == 0)
			{
				float degree = 90.f;
				Vec3 vMissilePos = Transform()->GetWorldPos() + Vec3(50.f * cosf(degree * 3.141592f / 180.f), 50.f * sinf(degree * 3.141592f / 180.f), 0.f);

				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
				Instantiate(pbullet, vMissilePos, 4);
				m_vecBullet.push_back(pbullet);
				++m_iMotion;
			}
		}
	}
}

void CBooklletScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Bullet" && m_eCurrentState != Bookllet_STATE::DEATH)
	{
		m_iHP -= (int)_pOther->GetOwner()->GetScript<CBulletScript>()->getDMG();
		if (m_eCurrentState != Bookllet_STATE::ATTACK)
		{
			float DIR = _pOther->GetOwner()->GetScript<CBulletScript>()->getDIR();
			RigidBody()->AddForce(Vec3(100.f * cosf(DIR * 3.141592f / 180.f), 100.f * sinf(DIR * 3.141592f / 180.f), 0.f));
		}
		_pOther->Destroy();
		if (m_iHP <= 0)
		{
			m_eCurrentState = Bookllet_STATE::DEATH;
			m_fAcctime = 0.f;
			Animator2D()->Play(L"DEATH", true);
		}
	}
}

void CBooklletScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Wall")
	{
		bool left = false, right = false, up = false, down = false;
		Vec2 Playerpos = Collider2D()->GetFinalPos();
		Vec2 Playerscale = Collider2D()->GetScale();

		Vec2 Otherpos = Vec2(_pOther->GetOwner()->GetParent()->Transform()->GetRelativePos().x + _pOther->GetOwner()->Transform()->GetRelativePos().x,
			_pOther->GetOwner()->GetParent()->Transform()->GetRelativePos().y + _pOther->GetOwner()->Transform()->GetRelativePos().y);
		Vec2 Otherscale = Vec2(_pOther->GetOwner()->Transform()->GetRelativeScale().x, _pOther->GetOwner()->Transform()->GetRelativeScale().y);

		float t, b, l, r;
		//겹쳐진 사각형 y 길이 구하기
		// top
		if (Playerpos.y + Playerscale.y / 2 >= Otherpos.y + Otherscale.y / 2)
		{
			t = Otherpos.y + Otherscale.y / 2;
			up = true;
		}
		else
		{
			t = Playerpos.y + Playerscale.y / 2;
			down = true;
		}
		// bottom
		if (Playerpos.y - Playerscale.y / 2 >= Otherpos.y - Otherscale.y / 2)
		{
			b = Playerpos.y - Playerscale.y / 2;
			up = true;
		}
		else
		{
			b = Otherpos.y - Otherscale.y / 2;
			down = true;
		}
		//겹쳐진 사각형 x 길이 구하기
		// right
		if (Playerpos.x + Playerscale.x / 2 >= Otherpos.x + Otherscale.x / 2)
		{
			r = Otherpos.x + Otherscale.x / 2;
			right = true;
		}
		else
		{
			r = Playerpos.x + Playerscale.x / 2;
			left = true;
		}
		// left
		if (Playerpos.x - Playerscale.x / 2 >= Otherpos.x - Otherscale.x / 2)
		{
			l = Playerpos.x - Playerscale.x / 2;
			right = true;
		}
		else
		{
			l = Otherpos.x - Otherscale.x / 2;
			left = true;
		}

		if (r - l >= t - b)
		{
			right = false;
			left = false;
		}
		else
		{
			up = false;
			down = false;
		}

		Vec3 vPos = Transform()->GetRelativePos();

		if (up)
		{
			vPos.y += t - b + 1;
		}
		if (down)
		{
			vPos.y -= t - b + 1;
		}
		if (left)
		{
			vPos.x -= r - l + 1;
		}
		if (right)
		{
			vPos.x += r - l + 1;
		}

		if (vPos != Transform()->GetRelativePos())
			Transform()->SetRelativePos(vPos);
	}

	if (_pOther->GetOwner()->GetName() == L"Monster")
	{
		bool left = false, right = false, up = false, down = false;
		Vec2 Playerpos = Collider2D()->GetFinalPos();
		Vec2 Playerscale = Collider2D()->GetScale();

		Vec2 Otherpos = _pOther->Collider2D()->GetFinalPos();
		Vec2 Otherscale = _pOther->Collider2D()->GetScale();

		float t, b, l, r;
		//겹쳐진 사각형 y 길이 구하기
		// top
		if (Playerpos.y + Playerscale.y / 2 >= Otherpos.y + Otherscale.y / 2)
		{
			t = Otherpos.y + Otherscale.y / 2;
			up = true;
		}
		else
		{
			t = Playerpos.y + Playerscale.y / 2;
			down = true;
		}
		// bottom
		if (Playerpos.y - Playerscale.y / 2 >= Otherpos.y - Otherscale.y / 2)
		{
			b = Playerpos.y - Playerscale.y / 2;
			up = true;
		}
		else
		{
			b = Otherpos.y - Otherscale.y / 2;
			down = true;
		}
		//겹쳐진 사각형 x 길이 구하기
		// right
		if (Playerpos.x + Playerscale.x / 2 >= Otherpos.x + Otherscale.x / 2)
		{
			r = Otherpos.x + Otherscale.x / 2;
			right = true;
		}
		else
		{
			r = Playerpos.x + Playerscale.x / 2;
			left = true;
		}
		// left
		if (Playerpos.x - Playerscale.x / 2 >= Otherpos.x - Otherscale.x / 2)
		{
			l = Playerpos.x - Playerscale.x / 2;
			right = true;
		}
		else
		{
			l = Otherpos.x - Otherscale.x / 2;
			left = true;
		}

		if (r - l >= t - b)
		{
			right = false;
			left = false;
		}
		else
		{
			up = false;
			down = false;
		}

		Vec3 vPos = Transform()->GetRelativePos();

		if (up)
		{
			vPos.y += t - b + 1;
		}
		if (down)
		{
			vPos.y -= t - b + 1;
		}
		if (left)
		{
			vPos.x -= r - l + 1;
		}
		if (right)
		{
			vPos.x += r - l + 1;
		}

		if (vPos != Transform()->GetRelativePos())
			Transform()->SetRelativePos(vPos);
	}
}

void CBooklletScript::EndOverlap(CCollider2D* _pOther)
{
}

void CBooklletScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
	//fwrite(&m_bullet, sizeof(Ptr<CPrefab>), 1, _File);
}

void CBooklletScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
	//fread(&m_bullet, sizeof(Ptr<CPrefab>), 1, _File);
}