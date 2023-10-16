#include "pch.h"
#include "CBlankScript.h"

CBlankScript::CBlankScript()
	: CScript(BLANKSCRIPT)
	, m_fAcctime(0.f)
{
}

CBlankScript::~CBlankScript()
{
}

void CBlankScript::begin()
{
}

void CBlankScript::tick()
{
	if (m_fAcctime > 0.5f)
		Destroy();

	Vec3 vScale = Transform()->GetRelativeScale();
	vScale.x += 5000.f * DT;
	vScale.y += 5000.f * DT;
	Transform()->SetRelativeScale(vScale);
	m_fAcctime += DT;
}

void CBlankScript::Overlap(CCollider2D* _pOther)
{
	_pOther->ReturnPool();
}

void CBlankScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CBlankScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}

