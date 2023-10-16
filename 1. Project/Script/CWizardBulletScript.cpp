#include "pch.h"
#include "CWizardBulletScript.h"

CWizardBulletScript::CWizardBulletScript()
	: CScript(WIZARDBULLETSCRIPT)
	, m_fSpeed(600.f)
	, m_fAcctime(0.f)
{
}

CWizardBulletScript::~CWizardBulletScript()
{
}

void CWizardBulletScript::tick()
{
	m_fAcctime += DT;
	
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += m_vDIR.x * m_fSpeed * DT;
	vPos.y += m_vDIR.y * m_fSpeed * DT;
	Transform()->SetRelativePos(vPos);

	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.z += 1.5f * DT * XM_PI;
	Transform()->SetRelativeRotation(vRot);

	if (m_fAcctime > 5.f)
	{
		GetOwner()->DisconnectChild();
		Destroy();
	}
}

void CWizardBulletScript::SaveToFile(FILE* _File)
{
}

void CWizardBulletScript::LoadFromFile(FILE* _File)
{
}
