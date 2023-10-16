#include "pch.h"
#include "CWizardScript.h"

#include "CMonsterBulletScript.h"
#include "CRoomScript.h"
#include "CBulletScript.h"
#include "CWizardBulletScript.h"
#include "CPlayerScript.h"

CWizardScript::CWizardScript()
	: CScript(WIZARDSCRIPT)
	, m_iHP(31)
	, m_eCurrentState(Wizard_STATE::IDLE)
	, m_pPlayer(nullptr)
	, m_pRoom(nullptr)
	, m_fAcctime(0.f)
	, m_fHittime(0.f)
	, m_iAttackMotion(0)
	, m_iMotion(2)
	, m_bInvi(false)
{
}

CWizardScript::~CWizardScript()
{
}

float CWizardScript::getPlayerDegree()
{
	Vec3 playerpos = m_pPlayer->Transform()->GetWorldPos();
	playerpos.y -= 30.f;
	playerpos.z = 0.f;
	Vec3 monsterpos = Transform()->GetWorldPos();
	Vec3 vDIR = playerpos - monsterpos;
	float degree = atan2((playerpos.y - monsterpos.y), (playerpos.x - monsterpos.x)) * 180 / 3.141592f;
	return degree;
}

void CWizardScript::tick()
{
	if (m_pPlayer == nullptr)
		return;
	if (m_eCurrentState == Wizard_STATE::DEATH)
	{
		if (m_iMotion == 10)
			return;
		else if (m_fAcctime >= 0.5f)
		{
			m_pRoom->GetScript<CRoomScript>()->MonsterDie();
			m_pPlayer->GetScript<CPlayerScript>()->addGold(10);
			GetOwner()->DelComponent(COMPONENT_TYPE::COLLIDER2D);
			Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(0.f, 0.f, 1.f));
			m_iMotion = 10;
		}
		else
		{
			m_fAcctime += DT;
		}
	}
	else if (m_eCurrentState == Wizard_STATE::IDLE)
	{
		if (m_fAcctime > 3.f)
		{
			if (m_iMotion == 3)
			{
				m_eCurrentState = Wizard_STATE::TELEPORT;
				float degree = getPlayerDegree();
				if (-90.f <= degree && degree < 90.f)
					Animator2D()->Play(L"tele_R", false);
				else
					Animator2D()->Play(L"tele_L", false);
				m_fAcctime = 0.f;
				m_iMotion = 0;
			}
			else
			{
				m_eCurrentState = Wizard_STATE::ATTACK;
				Animator2D()->Play(L"Attack", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\wizard_charge_01.wav");
				pSound->Play(1, 0.6f, true);
				m_fAcctime = 0.f;
				m_iAttackMotion = rand() % 3;
				++m_iMotion;
			}
		}
		else
		{
			m_fAcctime += DT;
			float degree = getPlayerDegree();
			if(0.f <= degree && degree < 90.f)
				Animator2D()->Play(L"IDLE_BR", true);
			else if (90.f <= degree && degree < 180.f)
				Animator2D()->Play(L"IDLE_BL", true);
			else if (-90.f <= degree && degree < 0.f)
				Animator2D()->Play(L"IDLE_FR", true);
			else // if (-180.f <= degree && degree < -90.f)
				Animator2D()->Play(L"IDLE_FL", true);
		}
	}
	else if (m_eCurrentState == Wizard_STATE::ATTACK)
	{
		m_fAcctime += DT;
		if (m_fAcctime > 1.5f)
		{
			m_eCurrentState = Wizard_STATE::IDLE;
			m_fAcctime = 0.f;
			m_fHittime = 0.f;
		}
		if (m_fAcctime > 1.3f && m_fHittime == 0.f)
		{
			m_fHittime += DT;
			CGameObject* pMissile = new CGameObject;
			pMissile->AddComponent(new CTransform);
			pMissile->AddComponent(new CWizardBulletScript);

			if (m_iAttackMotion == 0)
			{
				for (int i = 0; i < 36; ++i)
				{
					CGameObject* pbullet = g_vecMonsterBullet.back();
					g_vecMonsterBullet.pop_back();

					pbullet->GetScript<CMonsterBulletScript>()->setSpeed(0.f);
					pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 1.f, 1.f));
					pbullet->GetScript<CMonsterBulletScript>()->Fire();
					pbullet->Transform()->SetRelativePos(Vec3(100.f * cosf(i * 10.f * 3.141592f / 180.f), 100.f * sinf(i * 10.f * 3.141592f / 180.f), 0.f));
					pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
					pMissile->AddChild(pbullet);
				}
			}
			else if (m_iAttackMotion == 1)
			{
				for (int i = 0; i < 11; ++i)
				{
					CGameObject* pbullet = g_vecMonsterBullet.back();
					g_vecMonsterBullet.pop_back();

					pbullet->GetScript<CMonsterBulletScript>()->setSpeed(0.f);
					pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 1.f, 1.f));
					pbullet->GetScript<CMonsterBulletScript>()->Fire();
					pbullet->Transform()->SetRelativePos(Vec3(0.f, i*15.f-75.f, 0.f));
					pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
					pMissile->AddChild(pbullet);
				}
				for (int i = 0; i < 5; ++i)
				{
					CGameObject* pbullet = g_vecMonsterBullet.back();
					g_vecMonsterBullet.pop_back();

					pbullet->GetScript<CMonsterBulletScript>()->setSpeed(0.f);
					pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 1.f, 1.f));
					pbullet->GetScript<CMonsterBulletScript>()->Fire();
					pbullet->Transform()->SetRelativePos(Vec3(i * 15.f - 75.f, 0.f, 0.f));
					pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
					pMissile->AddChild(pbullet);
				}
				for (int i = 0; i < 5; ++i)
				{
					CGameObject* pbullet = g_vecMonsterBullet.back();
					g_vecMonsterBullet.pop_back();

					pbullet->GetScript<CMonsterBulletScript>()->setSpeed(0.f);
					pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 1.f, 1.f));
					pbullet->GetScript<CMonsterBulletScript>()->Fire();
					pbullet->Transform()->SetRelativePos(Vec3(i * 15.f + 15.f, 0.f, 0.f));
					pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
					pMissile->AddChild(pbullet);
				}
			}
			else if (m_iAttackMotion == 2)
			{
				for (int i = 0; i < 11; ++i)
				{
					CGameObject* pbullet = g_vecMonsterBullet.back();
					g_vecMonsterBullet.pop_back();
					pbullet->GetScript<CMonsterBulletScript>()->setSpeed(0.f);
					pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 1.f, 1.f));
					pbullet->GetScript<CMonsterBulletScript>()->Fire();
					pbullet->Transform()->SetRelativePos(Vec3(75.f, i * 15.f - 75.f, 0.f));
					pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
					pMissile->AddChild(pbullet);
				}
				for (int i = 0; i < 11; ++i)
				{
					CGameObject* pbullet = g_vecMonsterBullet.back();
					g_vecMonsterBullet.pop_back();
					pbullet->GetScript<CMonsterBulletScript>()->setSpeed(0.f);
					pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 1.f, 1.f));
					pbullet->GetScript<CMonsterBulletScript>()->Fire();
					pbullet->Transform()->SetRelativePos(Vec3(-75.f, i * 15.f - 75.f, 0.f));
					pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
					pMissile->AddChild(pbullet);
				}
				for (int i = 0; i < 9; ++i)
				{
					CGameObject* pbullet = g_vecMonsterBullet.back();
					g_vecMonsterBullet.pop_back();
					pbullet->GetScript<CMonsterBulletScript>()->setSpeed(0.f);
					pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 1.f, 1.f));
					pbullet->GetScript<CMonsterBulletScript>()->Fire();
					pbullet->Transform()->SetRelativePos(Vec3(i * 15.f - 60.f, 75.f, 0.f));
					pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
					pMissile->AddChild(pbullet);
				}
				for (int i = 0; i < 9; ++i)
				{
					CGameObject* pbullet = g_vecMonsterBullet.back();
					g_vecMonsterBullet.pop_back();
					pbullet->GetScript<CMonsterBulletScript>()->setSpeed(0.f);
					pbullet->GetScript<CMonsterBulletScript>()->setDIR(Vec3(1.f, 1.f, 1.f));
					pbullet->GetScript<CMonsterBulletScript>()->Fire();
					pbullet->Transform()->SetRelativePos(Vec3(i * 15.f - 60.f, -75.f, 0.f));
					pbullet->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 1.f));
					pMissile->AddChild(pbullet);
				}
			}

			pMissile->GetScript<CWizardBulletScript>()->setDIR(Vec3(cosf(getPlayerDegree() * 3.141592f / 180.f), sinf(getPlayerDegree() * 3.141592f / 180.f), 0.f));
			Instantiate(pMissile, Transform()->GetRelativePos(), 4);

			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\wizardred_shoot_0" + to_wstring((rand() % 2) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
			pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\wizard_shockwave_01.wav");
			pSound->Play(1, 0.6f, true);
		}
		
	}
	else if (m_eCurrentState == Wizard_STATE::TELEPORT)
	{
		m_fAcctime += DT;

		if (m_fAcctime > 2.4f)
		{
			m_eCurrentState = Wizard_STATE::IDLE;
			m_fAcctime = 0.f;
			m_iMotion = 0;
		}
		else if (m_fAcctime > 1.8f && m_iMotion == 1)
		{
			Vec2 vPos = m_pRoom->GetScript<CRoomScript>()->FindTele();
			Transform()->SetRelativePos(Vec3(vPos.x, vPos.y, 75.f));
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\wizardred_appear_01.wav");
			pSound->Play(1, 0.6f, true);
			CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"teleport_dustup")->Instantiate();
			impact->Animator2D()->Play(L"teleport_dustup", false);
			Vec3 vPos3 = Transform()->GetRelativePos();
			vPos3.z = 70.f;
			Instantiate(impact, vPos3, 0);
			float degree = getPlayerDegree();
			if (-90.f <= degree && degree < 90.f)
				Animator2D()->Play(L"tele_return_R", false);
			else
				Animator2D()->Play(L"tele_return_L", false);
			m_bInvi = false;
			++m_iMotion;
		}
		else if (m_fAcctime > 0.6f && m_iMotion == 0)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\wizardred_vanish_01.wav");
			pSound->Play(1, 0.6f, true);
			m_bInvi = true;
			CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"teleport_dustup")->Instantiate();
			impact->Animator2D()->Play(L"teleport_dustup", false);
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.z = 70.f;
			Instantiate(impact, vPos, 0);
			++m_iMotion;
		}
	}
	else if (m_eCurrentState == Wizard_STATE::HIT)
	{
		m_fAcctime += DT;
		if (m_fHittime >= 0.3f)
		{
			m_eCurrentState = Wizard_STATE::IDLE;
		}
	}
	else if (m_eCurrentState == Wizard_STATE::FALL)
	{
		m_fAcctime += DT;
		if (m_fAcctime >= 2.f && !GetOwner()->IsDead())
		{
			m_pPlayer->GetScript<CPlayerScript>()->addGold(10);
			m_pRoom->GetScript<CRoomScript>()->MonsterDie();
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

void CWizardScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Bullet" && m_eCurrentState != Wizard_STATE::DEATH && !m_bInvi)
	{
		m_iHP -= (int)_pOther->GetOwner()->GetScript<CBulletScript>()->getDMG();
		float DIR = _pOther->GetOwner()->GetScript<CBulletScript>()->getDIR();
		RigidBody()->AddForce(Vec3(100.f * cosf(DIR * 3.141592f / 180.f), 100.f * sinf(DIR * 3.141592f / 180.f), 0.f));
		_pOther->Destroy();
		if (m_iHP <= 0)
		{
			m_bInvi = true;
			m_eCurrentState = Wizard_STATE::DEATH;
			m_fAcctime = 0.f;
			if (getPlayerDegree() <= 90.f && getPlayerDegree() >= -90.f)
				Animator2D()->Play(L"Death_R", false);
			else
				Animator2D()->Play(L"Death_L", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\wizardred_death_0" + to_wstring((rand() % 2) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
		}
		else if(m_eCurrentState == Wizard_STATE::IDLE && m_fHittime == 0.f)
		{
			m_eCurrentState = Wizard_STATE::HIT;
			if (getPlayerDegree() <= 90.f || getPlayerDegree() >= -90.f)
				Animator2D()->Play(L"Hit_R", true);
			else
				Animator2D()->Play(L"Hit_L", true);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\wizardred_hurt_0" + to_wstring((rand() % 2) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
			m_fHittime += DT;
		}
	}
	if (_pOther->GetOwner()->GetName() == L"Cliff")
	{
		m_eCurrentState = Wizard_STATE::FALL;
		m_fAcctime = 0.f;
		m_bInvi = true;
		Animator2D()->Play(L"FALL", false);
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
		pSound->Play(1, 0.6f, true);
	}
}

void CWizardScript::Overlap(CCollider2D* _pOther)
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

void CWizardScript::EndOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Cliff")
	{
		if (m_eCurrentState == Wizard_STATE::FALL)
		{
			Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(0.f, 0.f, 1000.f));
		}
	}
}

void CWizardScript::SaveToFile(FILE* _File)
{
}

void CWizardScript::LoadFromFile(FILE* _File)
{
}


