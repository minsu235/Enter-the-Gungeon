#include "pch.h"
#include "CShotgunBulletKinScript.h"

#include "CMonsterBulletScript.h"
#include "CRoomScript.h"
#include "CBulletScript.h"
#include "CMonsterGun.h"
#include "CPlayerScript.h"

CShotgunBulletKinScript::CShotgunBulletKinScript()
	: CScript(SHOTGUNBULLETKINSCRIPT)
	, m_iHP(40)
	, m_eCurrentState(ShotgunBulletKin_STATE::IDLE)
	, m_pPlayer(nullptr)
	, m_pRoom(nullptr)
	, m_fSpeed(100.f)
	, m_fAcctime(0.f)
	, m_fWalktime(0.f)
	, m_iMotion(0)
	, m_fHittime(0.f)
{
}

CShotgunBulletKinScript::~CShotgunBulletKinScript()
{
}

float CShotgunBulletKinScript::getPlayerDegree()
{
	Vec3 playerpos = m_pPlayer->Transform()->GetWorldPos();
	playerpos.y -= 30.f;
	playerpos.z = 0.f;
	Vec3 monsterpos = Transform()->GetWorldPos();
	Vec3 vDIR = playerpos - monsterpos;
	float degree = atan2((playerpos.y - monsterpos.y), (playerpos.x - monsterpos.x)) * 180 / 3.141592f;
	return degree;
}

void CShotgunBulletKinScript::tick()
{
	if (m_pPlayer == nullptr)
		return;
	if (m_eCurrentState == ShotgunBulletKin_STATE::DEATH)
	{
		if (m_fAcctime >= 1.183f && !GetOwner()->IsDead())
		{
			m_pRoom->GetScript<CRoomScript>()->MonsterDie();
			m_pPlayer->GetScript<CPlayerScript>()->addGold(6);
			Destroy();
		}
		else if (m_fAcctime >= 0.85f && m_iMotion != 10)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shotgun_die_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
			m_iMotion = 10;
			Vec3 vPos = Transform()->GetRelativePos();
			for (int i = 0; i < 6; ++i)
			{
				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				pbullet->GetScript<CMonsterBulletScript>()->setSpeed(700.f);
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf(i * 60.f * 3.141592f / 180.f), sinf(i * 60.f * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);	
			}
		}
		else if (m_fAcctime >= 0.6f)
		{
			Animator2D()->Play(L"Death", false);
		}
			m_fAcctime += DT;
	}
	else if (m_eCurrentState == ShotgunBulletKin_STATE::IDLE)
	{
		if (m_fAcctime > 5.f)
		{
			m_iMotion = 0;
			m_fAcctime = 0.f;
			GetOwner()->GetChildObject()[0]->GetChildObject()[0]->GetScript<CMonsterGun>()->Fire();
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shotgun_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
		}
		else
		{
			m_fAcctime += DT;
			float degree = getPlayerDegree();
			if (0 <= degree && degree < 180.f)
				Animator2D()->Play(L"IDLE_B", true);
			else // if (-180.f <= degree && degree < -90.f)
				Animator2D()->Play(L"IDLE_F", true);

			Vec3 playerpos = m_pPlayer->Transform()->GetWorldPos();
			playerpos.y -= 30.f;
			playerpos.z = 0.f;
			Vec3 monsterpos = Transform()->GetWorldPos();
			monsterpos.z = 0.f;
			float distance = Vec3::Distance(playerpos, monsterpos);

			if (distance > 100.f)
			{
				m_eCurrentState = ShotgunBulletKin_STATE::WALK;
				m_iMotion = 0;
			}
		}
	}
	else if (m_eCurrentState == ShotgunBulletKin_STATE::WALK)
	{
		if (m_fAcctime > 5.f)
		{
			m_iMotion = 0;
			m_fAcctime = 0.f;
			GetOwner()->GetChildObject()[0]->GetChildObject()[0]->GetScript<CMonsterGun>()->Fire();
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shotgun_shot_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
		}
		else
		{
			m_fAcctime += DT;
			m_fWalktime += DT;

			if (RigidBody()->GetVelocity() == Vec3(0.f, 0.f, 0.f))
			{
				if (m_pPlayer != nullptr)
				{
					Vec3 playerpos = m_pPlayer->Transform()->GetWorldPos();
					playerpos.y -= 30.f;
					playerpos.z = 0.f;
					Vec3 monsterpos = Transform()->GetWorldPos();
					monsterpos.z = 0.f;
					float distance = Vec3::Distance(playerpos, monsterpos);
					if (distance <= 100.f)
					{
						m_eCurrentState = ShotgunBulletKin_STATE::IDLE;
						m_iMotion = 0;
					}
					else
					{
						Vec2 playerpos = m_pRoom->GetScript<CRoomScript>()->FindPath(GetOwner());

						Vec3 Dirrection = playerpos - monsterpos;
						Dirrection.Normalize();
						Vec3 vPos = Transform()->GetRelativePos();
						vPos.x += Dirrection.x * m_fSpeed * DT;
						vPos.y += Dirrection.y * m_fSpeed * DT;
						Transform()->SetRelativePos(vPos);

						float degree = atan2(Dirrection.y, Dirrection.x) * 180 / 3.141592f;;
						int i = 0;
						if (-67.5f <= degree && degree < 22.5f)
						{
							Animator2D()->Play(L"WALK_R", true);
							i = 1;
						}
						else if (22.5f <= degree && degree < 90.f)
						{
							Animator2D()->Play(L"WALK_BR", true);
							i = 2;
						}
						else if (90.f <= degree && degree < 157.5f)
						{
							Animator2D()->Play(L"WALK_BL", true);
							i = 3;
						}
						else if (-112.5f <= degree && degree < -67.5f)
						{
							Animator2D()->Play(L"WALK_F", true);
							i = 4;
						}
						else // if (-180.f <= degree && degree < -90.f)
						{
							Animator2D()->Play(L"WALK_L", true);
							i = 5;
						}
						if (m_iMotion != i)
						{
							m_iMotion = i;
							m_fWalktime = 0.f;
						}
						if (m_fWalktime > 0.375f)
						{
							Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\metalBullet_step_0" + to_wstring((rand() % 3) + 1) + L".wav");
							pSound->Play(1, 0.6f, true);
							m_fWalktime = 0.f;
						}
					}
				}
			}
		}
	}
	else if (m_eCurrentState == ShotgunBulletKin_STATE::FALL)
	{
		m_fAcctime += DT;
		if (m_fAcctime >= 2.f && !GetOwner()->IsDead())
		{
			m_pRoom->GetScript<CRoomScript>()->MonsterDie();
			m_pPlayer->GetScript<CPlayerScript>()->addGold(6);
			Destroy();
		}
	}
	if (m_fHittime != 0.f)
	{
		m_fHittime += DT;
		if (m_fHittime > 1.f)
			m_fHittime = 0.f;
	}
}

void CShotgunBulletKinScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Bullet" && m_eCurrentState != ShotgunBulletKin_STATE::DEATH && m_eCurrentState != ShotgunBulletKin_STATE::FALL)
	{
		m_iHP -= (int)_pOther->GetOwner()->GetScript<CBulletScript>()->getDMG();
		float DIR = _pOther->GetOwner()->GetScript<CBulletScript>()->getDIR();
		RigidBody()->AddForce(Vec3(100.f * cosf(DIR * 3.141592f / 180.f), 100.f * sinf(DIR * 3.141592f / 180.f), 0.f));
		_pOther->Destroy();
		if (m_iHP <= 0)
		{
			m_eCurrentState = ShotgunBulletKin_STATE::DEATH;
			m_fAcctime = 0.f;
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shotgun_death_0" + to_wstring((rand() % 2) + 1) + L".wav");
			pSound->Play(1, 0.5f, true);
			Animator2D()->Play(L"IDLE_F", true);
		}
		else if ((m_eCurrentState == ShotgunBulletKin_STATE::IDLE || m_eCurrentState == ShotgunBulletKin_STATE::WALK)&& m_fHittime == 0.f)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\shotgun_hurt_0" + to_wstring((rand() % 2) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
			m_fHittime += DT;
		}
	}
	if (_pOther->GetOwner()->GetName() == L"Cliff")
	{
		m_eCurrentState = ShotgunBulletKin_STATE::FALL;
		m_fAcctime = 0.f;
		Animator2D()->Play(L"FALL", false);
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
		pSound->Play(1, 0.6f, true);
	}
}

void CShotgunBulletKinScript::Overlap(CCollider2D* _pOther)
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

void CShotgunBulletKinScript::EndOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Cliff")
	{
		if (m_eCurrentState == ShotgunBulletKin_STATE::FALL)
		{
			Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(0.f, 0.f, 1000.f));
		}
	}
}

void CShotgunBulletKinScript::SaveToFile(FILE* _File)
{
}

void CShotgunBulletKinScript::LoadFromFile(FILE* _File)
{
}