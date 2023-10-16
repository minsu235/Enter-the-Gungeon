#include "pch.h"
#include "CBulletScript.h"

CBulletScript::CBulletScript()
	: CScript(BULLETSCRIPT)
	, m_iDMG(6)
	, m_fDIR(0.f)
	, m_fSpeed(700.f)
	, m_fAcctime(0.f)
	, m_fDuration(0.f)
	, m_gun(GUN::rusty_sidearm)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::begin()
{
}

void CBulletScript::tick()
{
	if (m_fDuration != 0.f)
	{
		m_fAcctime += DT;

		if (m_fAcctime >= m_fDuration)
		{
			CGameObject* bullet_impact = CResMgr::GetInst()->FindRes<CPrefab>(L"bullet_impact")->Instantiate();
			bullet_impact->Animator2D()->Play(L"bullet_impact", false);
			Instantiate(bullet_impact, Transform()->GetRelativePos(), 0);
			m_fAcctime = 0.f;
			Destroy();
		}
	}
	Vec3 vRot = Vec3(0.f, 0.f, m_fDIR * 3.141592f / 180);
	Transform()->SetRelativeRotation(vRot);
	Collider2D()->SetRotation(vRot);
	Vec3 vPos = Transform()->GetRelativePos();
	
	vPos.x += m_fSpeed * cosf(m_fDIR * 3.141592f / 180.f) * DT;
	vPos.y += m_fSpeed * sinf(m_fDIR * 3.141592f / 180.f) * DT;

	if (vPos != Transform()->GetRelativePos())
		Transform()->SetRelativePos(vPos);
}

void CBulletScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CBulletScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Wall")
	{
		if(m_gun == GUN::megahand)
		{ 
			if (m_iDMG == 45)
			{
				CGameObject* bullet_impact = CResMgr::GetInst()->FindRes<CPrefab>(L"megabuster_impact")->Instantiate();
				bullet_impact->Animator2D()->Play(L"megabuster_impact", false);
				Instantiate(bullet_impact, Transform()->GetRelativePos(), 0);
			}
			else
			{
				CGameObject* bullet_impact = CResMgr::GetInst()->FindRes<CPrefab>(L"bullet_impact")->Instantiate();
				bullet_impact->Animator2D()->Play(L"bullet_impact", false);
				Instantiate(bullet_impact, Transform()->GetRelativePos(), 0);
			}
			Destroy();
		}
		else
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

			if (up)
			{
				CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_wall")->Instantiate();
				impact->Animator2D()->Play(L"dustup_wall", false);
				impact->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 90.f / 180.f * 3.141592f));
				Vec3 vPos = Transform()->GetRelativePos();
				vPos.y += 16.f;

				Instantiate(impact, vPos, 0);
			}
			if (down)
			{
				CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_wall")->Instantiate();
				impact->Animator2D()->Play(L"dustup_wall", false);
				impact->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 270.f / 180.f * 3.141592f));
				Vec3 vPos = Transform()->GetRelativePos();
				vPos.y += 32.f;

				Instantiate(impact, vPos, 0);
			}
			if (left)
			{
				CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_wall")->Instantiate();
				impact->Animator2D()->Play(L"dustup_wall", false);
				impact->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 180.f / 180.f * 3.141592f));
				Vec3 vPos = Transform()->GetRelativePos();
				vPos.x -= 16.f;

				Instantiate(impact, vPos, 0);
			}
			if (right)
			{
				CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_wall")->Instantiate();
				impact->Animator2D()->Play(L"dustup_wall", false);
				Vec3 vPos = Transform()->GetRelativePos();
				vPos.x += 16.f;

				Instantiate(impact, vPos, 0);
			}

			Destroy();
			if (m_gun == GUN::rusty_sidearm || m_gun == GUN::shotgun)
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\metalbullet_impact_0" + to_wstring((rand() % 3) + 1) + L".wav");
				pSound->Play(1, 0.6f, true);
			}
		}
	}
}

void CBulletScript::EndOverlap(CCollider2D* _pOther)
{
}

void CBulletScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CBulletScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}

