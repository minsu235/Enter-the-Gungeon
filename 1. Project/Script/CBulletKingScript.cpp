#include "pch.h"
#include "CBulletKingScript.h"
#include "CBulletScript.h"
#include "CRoomScript.h"
#include "CMonsterBulletScript.h"
#include "CPlayerScript.h"
#include "CUIScript.h"
#include "CGameCameraScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CRenderMgr.h>

CBulletKingScript::CBulletKingScript()
	: CScript(BULLETKINGSCRIPT)
	, m_iHP(200)
	, m_eCurrentState(BulletKing_STATE::STOP)
	, m_pPlayer(nullptr)
	, m_fSpeed(100.f)
	, m_fAcctime(0.f)
	, m_iMotion(0)
	, m_iAct(0)
	, m_iDIR(0)
	, m_b3attack(false)
{
}

CBulletKingScript::~CBulletKingScript()
{
}

float CBulletKingScript::getPlayerDegree()
{
	Vec3 playerpos = m_pPlayer->Transform()->GetWorldPos();
	playerpos.y -= 30.f;
	playerpos.z = 0.f;
	Vec3 monsterpos = Transform()->GetWorldPos();
	Vec3 vDIR = playerpos - monsterpos;
	float degree = atan2((playerpos.y - monsterpos.y), (playerpos.x - monsterpos.x)) * 180 / 3.141592f;
	return degree;
}

void CBulletKingScript::tick()
{
	if (m_eCurrentState == BulletKing_STATE::DEATH)
	{
		if (m_fAcctime > 2.375f && m_iAct == 999)
		{
			CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"explosion_big")->Instantiate();
			impact->Animator2D()->Play(L"explosion_big", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_explode_01.wav");
			pSound->Play(1, 0.6f, true);
			Vec3 vPos3 = Transform()->GetRelativePos();
			vPos3.z = 70.f;
			Instantiate(impact, vPos3, 0);
			++m_iAct;
			GetOwner()->SetName(L"Monster_die");
			vPos3.z = 77.f;
			Transform()->SetRelativePos(vPos3);
		}
		else if (m_iAct == 1000)
			return;
		m_fAcctime += DT;
		
	}
	else if (m_eCurrentState == BulletKing_STATE::MOVE)
	{
		Vec3 playerpos = m_pPlayer->Transform()->GetWorldPos();
		playerpos.y -= 30.f;
		playerpos.z = 0.f;
		Vec3 monsterpos = Transform()->GetWorldPos();
		monsterpos.z = 0.f;

		Vec3 Dirrection = playerpos - monsterpos;
		Dirrection.Normalize();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += Dirrection.x * m_fSpeed * DT;
		vPos.y += Dirrection.y * m_fSpeed * DT;
		Transform()->SetRelativePos(vPos);
	}

	if (m_iAct == 0)
	{
		m_fAcctime += DT;

		if (m_iMotion == 0)
		{
			Animator2D()->Play(L"IDLE", true);
			CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->BossCameraMove(Transform()->GetRelativePos());
			++m_iMotion;
		}
		else if (m_iMotion == 1 && m_fAcctime >= 2.0f)
		{
			Animator2D()->Play(L"INTRO", false);
			++m_iMotion;
		}
		else if (m_fAcctime >= 7.0f)
		{
			Animator2D()->Play(L"IDLE", true);
			m_eCurrentState = BulletKing_STATE::MOVE;
			++m_iAct;
			m_iMotion = 0;
			m_fAcctime = 0;
			m_pPlayer->GetScript<CPlayerScript>()->getUI()->GetScript<CUIScript>()->setBossHP((UINT)m_iHP);
		}
	}
	else if (m_iAct == 1)
	{
		m_fAcctime += DT;
		
		if (m_iMotion == 0)
		{
			Animator2D()->Play(L"ATTACK_INTRO", false);
			m_fAcctime = 0;
			++m_iMotion;
		}
		else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
		{
			Attack_spread();
		}
		else if (m_iMotion == 2 && m_fAcctime >= 0.75f)
		{
			Animator2D()->Play(L"IDLE", true);
			++m_iMotion;
		}
		else if (m_fAcctime > 2.f)
		{
			m_fAcctime = 0.f;
			m_iMotion = 0;
			++m_iAct;
		}
	}
	else if (m_iAct == 2)
	{
		m_fAcctime += DT;
		if (m_fAcctime > 6.f)
		{
			m_fAcctime = 0.f;
			m_iMotion = 0;
			++m_iAct;
		}
		else
		{
			Attack_spin();
		}
	}
	else if (m_iAct == 3)
	{
		m_fAcctime += DT;

		if (m_fAcctime > 1.5f)
		{
			m_fAcctime = 0.f;
			m_iMotion = 0;
			m_iDIR = 0;
			++m_iAct;
		}
		else
		{
			if (m_iDIR == 0)
			{
				float degree = getPlayerDegree();
				if (-45.f <= degree && degree < 45.f)
				{
					m_iDIR = 1;
					m_b3attack = true;
				}
				else if (-90.f <= degree && degree < -45.f)
				{
					m_iDIR = 2;
					m_b3attack = true;
				}
				else if (-135.f <= degree && degree < -90.f)
				{
					m_iDIR = 3;
					m_b3attack = true;
				}
				else if (135.f <= degree || degree < -135.f)
				{
					m_iDIR = 4;
					m_b3attack = true;
				}
				else
				{
					m_iDIR = 5;
					m_b3attack = false;
				}
			}

			if (m_iDIR == 1)
				Attack_3shoot_r();
			else if (m_iDIR == 2)
				Attack_3shoot_fr();
			else if (m_iDIR == 3)
				Attack_3shoot_fl();
			else if (m_iDIR == 4)
				Attack_3shoot_l();
			else if (m_iDIR == 5)
			{
				if (m_iMotion == 0)
				{
					Animator2D()->Play(L"ATTACK_INTRO", false);
					m_fAcctime = 0;
					++m_iMotion;
				}
				else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
				{
					Attack_spread();
				}
				else if (m_iMotion == 2 && m_fAcctime >= 0.75f)
				{
					Animator2D()->Play(L"IDLE", true);
					++m_iMotion;
				}
			}
		}
	}
	else if (m_iAct == 4)
	{
		m_fAcctime += DT;
		if (m_fAcctime > 2.f)
		{
			m_fAcctime = 0.f;
			m_iMotion = 0;
			++m_iAct;
		}
		else
		{
			Attack_pound();
		}
	}
	else if (m_iAct == 5)
	{
		if (!m_b3attack)
		{
			++m_iAct;
		}
		else
		{
			m_fAcctime += DT;

			if (m_iMotion == 0)
			{
				Animator2D()->Play(L"ATTACK_INTRO", false);
				m_fAcctime = 0;
				++m_iMotion;
			}
			else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
			{
				Attack_spread();
			}
			else if (m_iMotion == 2 && m_fAcctime >= 0.75f)
			{
				Animator2D()->Play(L"IDLE", true);
				++m_iMotion;
			}
			else if (m_fAcctime > 2.f)
			{
				m_fAcctime = 0.f;
				m_iMotion = 0;
				++m_iAct;
				m_b3attack = false;
			}
		}
	}
	else if (m_iAct == 6)
	{
		m_fAcctime += DT;

		if (m_fAcctime > 1.5f)
		{
			m_fAcctime = 0.f;
			m_iMotion = 0;
			m_iDIR = 0;
			++m_iAct;
		}
		else
		{
			if (m_iDIR == 0)
			{
				float degree = getPlayerDegree();
				if (-45.f <= degree && degree < 45.f)
				{
					m_iDIR = 1;
					m_b3attack = true;
				}
				else if (-90.f <= degree && degree < -45.f)
				{
					m_iDIR = 2;
					m_b3attack = true;
				}
				else if (-135.f <= degree && degree < -90.f)
				{
					m_iDIR = 3;
					m_b3attack = true;
				}
				else if (135.f <= degree || degree < -135.f)
				{
					m_iDIR = 4;
					m_b3attack = true;
				}
				else
				{
					m_iDIR = 5;
					m_b3attack = false;
				}
			}

			if (m_iDIR == 1)
				Attack_3shoot_r();
			else if (m_iDIR == 2)
				Attack_3shoot_fr();
			else if (m_iDIR == 3)
				Attack_3shoot_fl();
			else if (m_iDIR == 4)
				Attack_3shoot_l();
			else if (m_iDIR == 5)
			{
				if (m_iMotion == 0)
				{
					Animator2D()->Play(L"ATTACK_INTRO", false);
					m_fAcctime = 0;
					++m_iMotion;
				}
				else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
				{
					Attack_spread();
				}
				else if (m_iMotion == 2 && m_fAcctime >= 0.75f)
				{
					Animator2D()->Play(L"IDLE", true);
					++m_iMotion;
				}
			}
		}
	}
	else if (m_iAct == 7)
	{
		if (!m_b3attack)
		{
			m_fAcctime += DT;
			if (m_fAcctime > 2.f)
			{
				m_fAcctime = 0.f;
				m_iMotion = 0;
				m_iAct = 2;
			}
			else
			{
				Attack_pound();
			}
		}
		else
		{
			m_fAcctime += DT;

			if (m_iMotion == 0)
			{
				Animator2D()->Play(L"ATTACK_INTRO", false);
				m_fAcctime = 0;
				++m_iMotion;
			}
			else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
			{
				Attack_spread();
			}
			else if (m_iMotion == 2 && m_fAcctime >= 0.75f)
			{
				Animator2D()->Play(L"IDLE", true);
				++m_iMotion;
			}
			else if (m_fAcctime > 2.f)
			{
				m_fAcctime = 0.f;
				m_iMotion = 0;
				m_iAct = 2;
				m_b3attack = false;
			}
		}
	}

}

void CBulletKingScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Bullet" && m_eCurrentState != BulletKing_STATE::DEATH)
	{
		m_iHP -= (int)_pOther->GetOwner()->GetScript<CBulletScript>()->getDMG();
		if (m_iHP < 0)
			m_iHP = 0;
		m_pPlayer->GetScript<CPlayerScript>()->getUI()->GetScript<CUIScript>()->setBossHP((UINT)m_iHP);
		_pOther->Destroy();
		if (m_iHP <= 0)
		{
			vector<CGameObject*> vec = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(4)->GetParentObjects();
			vector<CGameObject*>::iterator iter = vec.begin();
			for (; iter != vec.end(); ++iter)
			{
				(*iter)->ReturnPool();
			}
			CLevelMgr::GetInst()->playBGM(L"sound\\bossclear.mp3");
			m_eCurrentState = BulletKing_STATE::DEATH;
			m_fAcctime = 0.f;
			Animator2D()->Play(L"DEATH", false);
			m_iAct = 999;
			
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\general_death_01.wav");
			pSound->Play(1, 0.6f, true);
			pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_spin_01.wav");
			pSound->Stop();
		}
	}
}

void CBulletKingScript::Overlap(CCollider2D* _pOther)
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
}

void CBulletKingScript::EndOverlap(CCollider2D* _pOther)
{
}

void CBulletKingScript::SaveToFile(FILE* _File)
{
}

void CBulletKingScript::LoadFromFile(FILE* _File)
{
}

void CBulletKingScript::Attack_spread()
{
	Animator2D()->Play(L"ATTACK", false);
	++m_iMotion;
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_sla01.wav");
	pSound->Play(1, 0.6f, true);
	float degree = 90.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (140 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (140 + 5 * i) * sinf(degree * 3.141592f / 180.f);
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = 70.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (130 + 5 * i) * cosf(degree * 3.141592f / 180.f) + 10.f;
		vPos.y += (130 + 5 * i) * sinf(degree * 3.141592f / 180.f);
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = 110.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (130 + 5 * i) * cosf(degree * 3.141592f / 180.f) - 10.f;
		vPos.y += (130 + 5 * i) * sinf(degree * 3.141592f / 180.f);
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = 40.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f) - 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = 140.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f) - 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = 10.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (110 + 5 * i) * sinf(degree * 3.141592f / 180.f) - 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = 170.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (110 + 5 * i) * sinf(degree * 3.141592f / 180.f) - 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -5.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (110 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = 185.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (110 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -15.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (110 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = 195.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (110 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -45.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 20.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -135.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f);
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 20.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(250.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -75.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f) + 50.f;
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 20.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -105.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f) - 50.f;
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 20.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -85.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f) + 60.f;
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -95.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f) - 60.f;
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -95.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f) + 70.f;
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f);
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}

	degree = -85.f;

	for (int i = 0; i < 4; ++i)
	{
		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120 + 5 * i) * cosf(degree * 3.141592f / 180.f) - 70.f;
		vPos.y += (120 + 5 * i) * sinf(degree * 3.141592f / 180.f);
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(degree * 3.141592f / 180.f), sinf(degree * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f + 30 * i);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
		Instantiate(pbullet, vPos, 4);
	}
}

void CBulletKingScript::Attack_3shoot_r()
{
	if (m_iMotion == 0)
	{
		Animator2D()->Play(L"ATTACK_INTRO", false);
		m_fAcctime = 0;
		++m_iMotion;
	}
	else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
	{
		Animator2D()->Play(L"ATTACK", false);
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += 120 * cosf(40 * 3.141592f / 180.f);
		vPos.y += 120 * sinf(40 * 3.141592f / 180.f) - 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(10.f * 3.141592f / 180.f), sinf(10.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 2 && m_fAcctime >= 0.475f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110) * cosf(10 * 3.141592f / 180.f);
		vPos.y += (110) * sinf(10 * 3.141592f / 180.f) - 15.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 0.f, 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 3 && m_fAcctime >= 0.575f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-45.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-45.f * 3.141592f / 180.f) + 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-10.f * 3.141592f / 180.f), sinf(-10.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 4 && m_fAcctime >= 0.75f)
	{
		++m_iMotion;
		Animator2D()->Play(L"IDLE", true);
	}
	else if (m_iMotion == 5 && m_fAcctime >= 0.805f)
	{
		Animator2D()->Play(L"ATTACK", false);
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += 120 * cosf(40 * 3.141592f / 180.f);
		vPos.y += 120 * sinf(40 * 3.141592f / 180.f) - 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(5.f * 3.141592f / 180.f), sinf(5.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 6 && m_fAcctime >= 0.905f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110) * cosf(10 * 3.141592f / 180.f);
		vPos.y += (110) * sinf(10 * 3.141592f / 180.f) - 15.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 0.f, 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 7 && m_fAcctime >= 1.005f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-45.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-45.f * 3.141592f / 180.f) + 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-5.f * 3.141592f / 180.f), sinf(-5.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 8 && m_fAcctime >= 1.125f)
	{
		++m_iMotion;
		Animator2D()->Play(L"IDLE", true);
	}
}

void CBulletKingScript::Attack_3shoot_l()
{
	if (m_iMotion == 0)
	{
		Animator2D()->Play(L"ATTACK_INTRO", false);
		m_fAcctime = 0;
		++m_iMotion;
	}
	else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
	{
		Animator2D()->Play(L"ATTACK", false);
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += 120 * cosf(140 * 3.141592f / 180.f);
		vPos.y += 120 * sinf(140 * 3.141592f / 180.f) - 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(170.f * 3.141592f / 180.f), sinf(170.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 2 && m_fAcctime >= 0.475f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110) * cosf(170 * 3.141592f / 180.f);
		vPos.y += (110) * sinf(170 * 3.141592f / 180.f) - 15.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(-1.f, 0.f, 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 3 && m_fAcctime >= 0.575f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-135.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-135.f * 3.141592f / 180.f) + 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-170.f * 3.141592f / 180.f), sinf(-170.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 4 && m_fAcctime >= 0.75f)
	{
		++m_iMotion;
		Animator2D()->Play(L"IDLE", true);
	}
	else if (m_iMotion == 5 && m_fAcctime >= 0.805f)
	{
		Animator2D()->Play(L"ATTACK", false);
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += 120 * cosf(140 * 3.141592f / 180.f);
		vPos.y += 120 * sinf(140 * 3.141592f / 180.f) - 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(175.f * 3.141592f / 180.f), sinf(175.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 6 && m_fAcctime >= 0.905f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (110) * cosf(170 * 3.141592f / 180.f);
		vPos.y += (110) * sinf(170 * 3.141592f / 180.f) - 15.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(-1.f, 0.f, 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 7 && m_fAcctime >= 1.005f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-135.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-135.f * 3.141592f / 180.f) + 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-175.f * 3.141592f / 180.f), sinf(-175.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 8 && m_fAcctime >= 1.125f)
	{
		++m_iMotion;
		Animator2D()->Play(L"IDLE", true);
	}
}

void CBulletKingScript::Attack_3shoot_fr()
{
	if (m_iMotion == 0)
	{
		Animator2D()->Play(L"ATTACK_INTRO", false);
		m_fAcctime = 0;
		++m_iMotion;
	}
	else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
	{
		Animator2D()->Play(L"ATTACK", false);
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-45.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-45.f * 3.141592f / 180.f) + 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-45.f * 3.141592f / 180.f), sinf(-45.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 2 && m_fAcctime >= 0.475f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-45.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-45.f * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-60.f * 3.141592f / 180.f), sinf(-60.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 3 && m_fAcctime >= 0.575f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-45.f * 3.141592f / 180.f) - 10.f;
		vPos.y += (120) * sinf(-45.f * 3.141592f / 180.f) + 5.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-75.f * 3.141592f / 180.f), sinf(-75.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 4 && m_fAcctime >= 0.75f)
	{
		++m_iMotion;
		Animator2D()->Play(L"IDLE", true);
	}
	else if (m_iMotion == 5 && m_fAcctime >= 0.805f)
	{
		Animator2D()->Play(L"ATTACK", false);
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-45.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-45.f * 3.141592f / 180.f) + 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-45.f * 3.141592f / 180.f), sinf(-45.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 6 && m_fAcctime >= 0.905f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-45.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-45.f * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-55.f * 3.141592f / 180.f), sinf(-55.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 7 && m_fAcctime >= 1.005f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-45.f * 3.141592f / 180.f) - 10.f;
		vPos.y += (120) * sinf(-45.f * 3.141592f / 180.f) + 5.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-65.f * 3.141592f / 180.f), sinf(-65.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 8 && m_fAcctime >= 1.125f)
	{
		++m_iMotion;
		Animator2D()->Play(L"IDLE", true);
	}
}

void CBulletKingScript::Attack_3shoot_fl()
{
	if (m_iMotion == 0)
	{
		Animator2D()->Play(L"ATTACK_INTRO", false);
		m_fAcctime = 0;
		++m_iMotion;
	}
	else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
	{
		Animator2D()->Play(L"ATTACK", false);
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-135.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-135.f * 3.141592f / 180.f) + 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-135.f * 3.141592f / 180.f), sinf(-135.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 2 && m_fAcctime >= 0.475f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-135.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-135.f * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-120.f * 3.141592f / 180.f), sinf(-120.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 3 && m_fAcctime >= 0.575f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-135.f * 3.141592f / 180.f) + 10.f;
		vPos.y += (120) * sinf(-135.f * 3.141592f / 180.f) + 5.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-105.f * 3.141592f / 180.f), sinf(-105.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 4 && m_fAcctime >= 0.75f)
	{
		++m_iMotion;
		Animator2D()->Play(L"IDLE", true);
	}
	else if (m_iMotion == 5 && m_fAcctime >= 0.805f)
	{
		Animator2D()->Play(L"ATTACK", false);
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-135.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-135.f * 3.141592f / 180.f) + 30.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-135.f * 3.141592f / 180.f), sinf(-135.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 6 && m_fAcctime >= 0.905f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-135.f * 3.141592f / 180.f);
		vPos.y += (120) * sinf(-135.f * 3.141592f / 180.f) + 10.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-125.f * 3.141592f / 180.f), sinf(-125.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 7 && m_fAcctime >= 1.005f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.6f, true);

		CGameObject* pbullet = g_vecMonsterBullet.back();
		g_vecMonsterBullet.pop_back();
		pbullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl"));
		pbullet->Transform()->SetRelativeScale(Vec3(60.f, 30.f, 0.f));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += (120) * cosf(-135.f * 3.141592f / 180.f) + 10.f;
		vPos.y += (120) * sinf(-135.f * 3.141592f / 180.f) + 5.f;
		pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(-115.f * 3.141592f / 180.f), sinf(-115.f * 3.141592f / 180.f), 0.f));
		pbullet->GetScript<CMonsterBulletScript>()->setSpeed(400.f);
		pbullet->GetScript<CMonsterBulletScript>()->Fire();
		Instantiate(pbullet, vPos, 4);
	}
	else if (m_iMotion == 8 && m_fAcctime >= 1.125f)
	{
		++m_iMotion;
		Animator2D()->Play(L"IDLE", true);
	}
}

void CBulletKingScript::Attack_pound()
{
	if (m_iMotion == 0)
	{
		Animator2D()->Play(L"POUND", false);
		m_eCurrentState = BulletKing_STATE::STOP;
		++m_iMotion;
		m_fAcctime = 0;
	}
	else if (m_iMotion == 1 && m_fAcctime >= 0.375f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_sla01.wav");
		pSound->Play(1, 0.6f, true);
		for (int i = 0; i < 36; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.y -= 100.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(10 * i * 3.141592f / 180.f), sinf(10 * i * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(350.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 2 && m_fAcctime >= 0.625)
	{
		Animator2D()->Play(L"IDLE", true);
		m_eCurrentState = BulletKing_STATE::MOVE;
		++m_iMotion;
	}
}

void CBulletKingScript::Attack_spin()
{
	if (m_iMotion == 0)
	{
		Animator2D()->Play(L"SPIN", false);
		m_eCurrentState = BulletKing_STATE::STOP;
		++m_iMotion;
		m_fAcctime = 0;
	}
	else if (m_iMotion == 1 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bulletking_spin_01.wav");
		pSound->Play(1, 0.6f, true);
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 2 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 3 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 4 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 5 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 6 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 7 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 8 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 9 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 10 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 11 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 12 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 13 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 14 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 15 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 16 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 17 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 18 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 19 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 20 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 21 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 22 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 23 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 24 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		float degree = (m_iMotion - 1) * 23.f;
		for (int i = 0; i < 6; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += (100) * cosf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.y += (100) * sinf((degree + 60.f * i) * 3.141592f / 180.f);
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 60.f * i) * 3.141592f / 180.f), sinf((degree + 60.f * i) * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(300.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 25 && m_fAcctime >= 0.375f + m_iMotion * 0.125f)
	{
		++m_iMotion;
		for (int i = 0; i < 36; ++i)
		{
			CGameObject* pbullet = g_vecMonsterBullet.back();
			g_vecMonsterBullet.pop_back();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.z = 70.f;
			pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(10 * i * 3.141592f / 180.f), sinf(10 * i * 3.141592f / 180.f), 0.f));
			pbullet->GetScript<CMonsterBulletScript>()->setSpeed(350.f);
			pbullet->GetScript<CMonsterBulletScript>()->Fire();
			pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
			Instantiate(pbullet, vPos, 4);
		}
	}
	else if (m_iMotion == 26 && m_fAcctime >= 4.625f)
	{
		Animator2D()->Play(L"IDLE", true);
		m_eCurrentState = BulletKing_STATE::MOVE;
		++m_iMotion;
	}
}
