#include "pch.h"
#include "CGunnutScript.h"

#include "CMonsterBulletScript.h"
#include "CRoomScript.h"
#include "CBulletScript.h"
#include "CPlayerScript.h"

CGunnutScript::CGunnutScript()
    : CScript(GUNNUTSCRIPT)
	, m_iHP(100)
	, m_eCurrentState(Gunnut_STATE::IDLE)
	, m_pPlayer(nullptr)
	, m_pRoom(nullptr)
	, m_fSpeed(100.f)
	, m_fAcctime(0.f)
	, m_fWalktime(0.f)
	, m_iMotion(0)
{
}

CGunnutScript::~CGunnutScript()
{
}

float CGunnutScript::getPlayerDegree()
{
	Vec3 playerpos = m_pPlayer->Transform()->GetWorldPos();
	playerpos.y -= 30.f;
	playerpos.z = 0.f;
	Vec3 monsterpos = Transform()->GetWorldPos();
	Vec3 vDIR = playerpos - monsterpos;
	float degree = atan2((playerpos.y - monsterpos.y), (playerpos.x - monsterpos.x)) * 180 / 3.141592f;
	return degree;
}

void CGunnutScript::tick()
{
	if (m_pPlayer == nullptr)
		return;
	if (m_eCurrentState == Gunnut_STATE::DEATH)
	{
		if (m_iMotion == 10)
			return;
		else if (m_fAcctime >= 0.5f)
		{
			m_pRoom->GetScript<CRoomScript>()->MonsterDie();
			m_pPlayer->GetScript<CPlayerScript>()->addGold(20);
			GetOwner()->DelComponent(COMPONENT_TYPE::COLLIDER2D);
			Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(0.f, 0.f, 1.f));
			m_iMotion = 10;
		}
		else
		{
			m_fAcctime += DT;
		}
	}
	else if (m_eCurrentState == Gunnut_STATE::IDLE)
	{
		if (m_fAcctime > 5.f)
		{
			m_iMotion = 0;
			m_fAcctime = 0.f;
			float degree = getPlayerDegree();
			if (-22.5f <= degree && degree < 22.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_R;
				Animator2D()->Play(L"Attack_R", false);
			}

			else if (22.5f <= degree && degree < 67.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_BR;
				Animator2D()->Play(L"Attack_BR", false);
			}
			else if (67.5f <= degree && degree < 112.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_B;
				Animator2D()->Play(L"Attack_B", false);
			}
			else if (112.5f <= degree && degree < 157.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_BL;
				Animator2D()->Play(L"Attack_BL", false);
			}
			else if (-67.5f <= degree && degree < -22.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_FR;
				Animator2D()->Play(L"Attack_FR", false);
			}
			else if (-112.5f <= degree && degree < -67.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_F;
				Animator2D()->Play(L"Attack_F", false);
			}
			else if (-157.5f <= degree && degree < -112.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_FL;
				Animator2D()->Play(L"Attack_FL", false);
			}
			else
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_L;
				Animator2D()->Play(L"Attack_L", false);
			}
		}
		else
		{
			m_fAcctime += DT;
			float degree = getPlayerDegree();
			if (45.f <= degree && degree < 135.f)
				Animator2D()->Play(L"IDLE_B", true);
			else if (-45.f <= degree && degree < 45.f)
				Animator2D()->Play(L"IDLE_R", true);
			else if (-135.f <= degree && degree < -45.f)
				Animator2D()->Play(L"IDLE_F", true);
			else // if (-180.f <= degree && degree < -90.f)
				Animator2D()->Play(L"IDLE_L", true);

			Vec3 playerpos = m_pPlayer->Transform()->GetWorldPos();
			playerpos.y -= 30.f;
			playerpos.z = 0.f;
			Vec3 monsterpos = Transform()->GetWorldPos();
			monsterpos.z = 0.f;
			float distance = Vec3::Distance(playerpos, monsterpos);

			if (distance > 100.f)
			{
				m_eCurrentState = Gunnut_STATE::WALK;
				m_iMotion = 0;
			}
		}
	}
	else if (m_eCurrentState == Gunnut_STATE::WALK)
	{
		if (m_fAcctime > 5.f)
		{
			m_iMotion = 0;
			m_fAcctime = 0.f;
			float degree = getPlayerDegree();
			if (-22.5f <= degree && degree < 22.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_R;
				Animator2D()->Play(L"Attack_R", false);
			}
			else if (22.5f <= degree && degree < 67.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_BR;
				Animator2D()->Play(L"Attack_BR", false);
			}
			else if (67.5f <= degree && degree < 112.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_B;
				Animator2D()->Play(L"Attack_B", false);
			}
			else if (112.5f <= degree && degree < 157.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_BL;
				Animator2D()->Play(L"Attack_BL", false);
			}
			else if (-67.5f <= degree && degree < -22.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_FR;
				Animator2D()->Play(L"Attack_FR", false);
			}
			else if (-112.5f <= degree && degree < -67.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_F;
				Animator2D()->Play(L"Attack_F", false);
			}
			else if (-157.5f <= degree && degree < -112.5f)
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_FL;
				Animator2D()->Play(L"Attack_FL", false);
			}
			else
			{
				m_eCurrentState = Gunnut_STATE::ATTACK_L;
				Animator2D()->Play(L"Attack_L", false);
			}
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
						m_eCurrentState = Gunnut_STATE::IDLE;
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
						if (45.f <= degree && degree < 135.f)
						{
							Animator2D()->Play(L"WALK_B", true);
							i = 1;
						}
						else if (-45.f <= degree && degree < 45.f)
						{
							Animator2D()->Play(L"WALK_R", true);
							i = 2;
						}
						else if (-135.f <= degree && degree < -45.f)
						{
							Animator2D()->Play(L"WALK_F", true);
							i = 3;
						}
						else // if (-180.f <= degree && degree < -90.f)
						{
							Animator2D()->Play(L"WALK_L", true);
							i = 4;
						}
						if (m_iMotion != i)
						{
							m_iMotion = i;
							m_fWalktime = 0.f;
						}
						if (m_fWalktime > 0.5f)
						{
							Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\gunnut_walk_0" + to_wstring((rand() % 3) + 1) + L".wav");
							pSound->Play(1, 0.6f, true);
							m_fWalktime = 0.f;
						}
					}
				}
			}
		}
	}
	else
	{
		m_fAcctime += DT;
		if (m_fAcctime >= 1.3125f)
		{
			m_fAcctime = 0.f;
			m_iMotion = 0;
			m_eCurrentState = Gunnut_STATE::IDLE;
		}
		else if (m_fAcctime >= 0.875f && m_iMotion == 1)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\gunnut_shockwave_0" + to_wstring((rand() % 2) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
			++m_iMotion;
			float degree = 0.f;
			Vec2 point;

			if (m_eCurrentState == Gunnut_STATE::ATTACK_B)
			{
				degree = 45.f;
				point = Vec2(0.f, 30.f);
			}
			else if (m_eCurrentState == Gunnut_STATE::ATTACK_F)
			{
				degree = 225.f;
				point = Vec2(0.f, -100.f);
			}
			else if (m_eCurrentState == Gunnut_STATE::ATTACK_L)
			{
				degree = 135.f;
				point = Vec2(-90.f, -70.f);
			}
			else if (m_eCurrentState == Gunnut_STATE::ATTACK_R)
			{
				degree = -45.f;
				point = Vec2(90.f, -70.f);
			}
			else if (m_eCurrentState == Gunnut_STATE::ATTACK_BL)
			{
				degree = 90.f;
				point = Vec2(-50.f, -20.f);
			}
			else if (m_eCurrentState == Gunnut_STATE::ATTACK_BR)
			{
				degree = 0.f;
				point = Vec2(30.f, -20.f);
			}
			else if (m_eCurrentState == Gunnut_STATE::ATTACK_FL)
			{
				degree = 180.f;
				point = Vec2(-90.f, -70.f);
			}
			else if (m_eCurrentState == Gunnut_STATE::ATTACK_FR)
			{
				degree = 270.f;
				point = Vec2(90.f, -70.f);
			}

			for (int i = 0; i < 25; ++i)
			{
				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				Vec3 vPos = Transform()->GetRelativePos();
				vPos.x += point.x;
				vPos.y += point.y;
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 3.75f * i) * 3.141592f / 180.f), sinf((degree + 3.75f * i) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				vPos.x += cosf((degree + 3.75f * i) * 3.141592f / 180.f);
				vPos.y += sinf((degree + 3.75f * i) * 3.141592f / 180.f);
				Instantiate(pbullet, vPos, 4);
			}

			for (int i = 0; i < 3; ++i)
			{
				CGameObject* pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				Vec3 vPos = Transform()->GetRelativePos();
				vPos.x += point.x;
				vPos.y += point.y;
				vPos.x += 80.f * cosf((degree + 45.f * i) * 3.141592f / 180.f);
				vPos.y += 80.f * sinf((degree + 45.f * i) * 3.141592f / 180.f);
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 45.f * i) * 3.141592f / 180.f), sinf((degree + 45.f * i) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);

				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				vPos = Transform()->GetRelativePos();
				vPos.x += point.x;
				vPos.y += point.y;
				vPos.x += 80.f * cosf((degree + 45.f * i) * 3.141592f / 180.f) + 10.f * 1.414213f * cosf((degree + 45.f * i + 135.f) * 3.141592f / 180.f);
				vPos.y += 80.f * sinf((degree + 45.f * i) * 3.141592f / 180.f) + 10.f * 1.414213f * sinf((degree + 45.f * i + 135.f) * 3.141592f / 180.f);
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 45.f * i) * 3.141592f / 180.f), sinf((degree + 45.f * i) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);

				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				vPos = Transform()->GetRelativePos();
				vPos.x += point.x;
				vPos.y += point.y;
				vPos.x += 80.f * cosf((degree + 45.f * i) * 3.141592f / 180.f) + 20.f * 1.414213f * cosf((degree + 45.f * i + 135.f) * 3.141592f / 180.f);
				vPos.y += 80.f * sinf((degree + 45.f * i) * 3.141592f / 180.f) + 20.f * 1.414213f * sinf((degree + 45.f * i + 135.f) * 3.141592f / 180.f);
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 45.f * i) * 3.141592f / 180.f), sinf((degree + 45.f * i) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);

				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				vPos = Transform()->GetRelativePos();
				vPos.x += point.x;
				vPos.y += point.y;
				vPos.x += 80.f * cosf((degree + 45.f * i) * 3.141592f / 180.f) + 10.f * 1.414213f * cosf((degree + 45.f * i - 135.f) * 3.141592f / 180.f);
				vPos.y += 80.f * sinf((degree + 45.f * i) * 3.141592f / 180.f) + 10.f * 1.414213f * sinf((degree + 45.f * i - 135.f) * 3.141592f / 180.f);
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 45.f * i) * 3.141592f / 180.f), sinf((degree + 45.f * i) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);

				pbullet = g_vecMonsterBullet.back();
				g_vecMonsterBullet.pop_back();
				vPos = Transform()->GetRelativePos();
				vPos.x += point.x;
				vPos.y += point.y;
				vPos.x += 80.f * cosf((degree + 45.f * i) * 3.141592f / 180.f) + 20.f * 1.414213f * cosf((degree + 45.f * i - 135.f) * 3.141592f / 180.f);
				vPos.y += 80.f * sinf((degree + 45.f * i) * 3.141592f / 180.f) + 20.f * 1.414213f * sinf((degree + 45.f * i - 135.f) * 3.141592f / 180.f);
				pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(cosf((degree + 45.f * i) * 3.141592f / 180.f), sinf((degree + 45.f * i) * 3.141592f / 180.f), 0.f));
				pbullet->GetScript<CMonsterBulletScript>()->Fire();
				pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
				Instantiate(pbullet, vPos, 4);
			}
		}
		else if (m_fAcctime >= 0.6875f && m_iMotion == 0)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\gunnut_swing_01.wav");
			pSound->Play(1, 0.6f, true);
			++m_iMotion;
		}
	}
}

void CGunnutScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Bullet" && m_eCurrentState != Gunnut_STATE::DEATH)
	{
		m_iHP -= (int)_pOther->GetOwner()->GetScript<CBulletScript>()->getDMG();
		float DIR = _pOther->GetOwner()->GetScript<CBulletScript>()->getDIR();
		RigidBody()->AddForce(Vec3(50.f * cosf(DIR * 3.141592f / 180.f), 50.f * sinf(DIR * 3.141592f / 180.f), 0.f));
		_pOther->Destroy();
		if (m_iHP <= 0)
		{
			m_eCurrentState = Gunnut_STATE::DEATH;
			m_fAcctime = 0.f;
			if (getPlayerDegree() <= 90.f && getPlayerDegree() >= -90.f)
				Animator2D()->Play(L"DIE_R", false);
			else
				Animator2D()->Play(L"DIE_L", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\general_death_01.wav");
			pSound->Play(1, 0.6f, true);
		}
	}
}

void CGunnutScript::Overlap(CCollider2D* _pOther)
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

void CGunnutScript::EndOverlap(CCollider2D* _pOther)
{
}

void CGunnutScript::SaveToFile(FILE* _File)
{
}

void CGunnutScript::LoadFromFile(FILE* _File)
{
}


