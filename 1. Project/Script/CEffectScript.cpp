#include "pch.h"
#include "CEffectScript.h"

CEffectScript::CEffectScript()
	: CScript(EFFECTSCRIPT)
	, m_fAcctime(0.f)
	, m_fEffecttime(0.f)
{
}

CEffectScript::~CEffectScript()
{
}

void CEffectScript::tick()
{
	if (m_fAcctime >= m_fEffecttime)
		Destroy();

	m_fAcctime += DT;
}

void CEffectScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CEffectScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}


