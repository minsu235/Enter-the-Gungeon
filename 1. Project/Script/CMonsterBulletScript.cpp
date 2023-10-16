#include "pch.h"
#include "CMonsterBulletScript.h"

CMonsterBulletScript::CMonsterBulletScript()
	: CScript(MONSTERBULLETSCRIPT)
	, m_iDMG(0)
	, m_fSpeed(700.f)
	, m_fAcctime(0.f)
	, m_bFire(false)
	, m_fWaitingtime(0.f)
{
}

CMonsterBulletScript::~CMonsterBulletScript()
{
}

void CMonsterBulletScript::reset()
{
	m_fAcctime = 0.f;
	m_bFire = false;
	m_fWaitingtime = 0.f;
	m_pPlayer = nullptr;
	m_fSpeed = 700.f;
}

void CMonsterBulletScript::begin()
{
}

void CMonsterBulletScript::tick()
{
	if (!m_bFire)
	{
		if (m_fWaitingtime > 0.f)
		{
			m_fAcctime += DT;
			if (m_fAcctime > m_fWaitingtime)
			{
				m_bFire = true;
			}
		}
	}
	else
	{
		if (m_vDIR == Vec3(0.f, 0.f, 0.f))
		{
			Vec3 playerpos = m_pPlayer->Transform()->GetWorldPos();
			playerpos.y -= 30.f;
			playerpos.z = 0.f;
			Vec3 bulletpos = Transform()->GetWorldPos();
			bulletpos.z = 0.f;
			m_vDIR = playerpos - bulletpos;
			m_vDIR.Normalize();
		}
		Vec3 vRot = Vec3(0.f, 0.f, atan2(m_vDIR.y, m_vDIR.x));
		Transform()->SetRelativeRotation(vRot);
		Collider2D()->SetRotation(vRot);
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += m_vDIR.x * m_fSpeed * DT;
		vPos.y += m_vDIR.y * m_fSpeed * DT;
		Transform()->SetRelativePos(vPos);
	}
}

void CMonsterBulletScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CMonsterBulletScript::Overlap(CCollider2D* _pOther)
{
	if (m_bFire == false)
		return;

	if (_pOther->GetOwner()->GetName() == L"Wall")
	{
		ReturnPool();
	}
}

void CMonsterBulletScript::EndOverlap(CCollider2D* _pOther)
{
}

void CMonsterBulletScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
	//fwrite(&m_bullet, sizeof(Ptr<CPrefab>), 1, _File);
}

void CMonsterBulletScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
	//fread(&m_bullet, sizeof(Ptr<CPrefab>), 1, _File);
}


