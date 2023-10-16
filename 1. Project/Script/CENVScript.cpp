#include "pch.h"
#include "CENVScript.h"

#include <Engine/CLevelMgr.h>

CENVScript::CENVScript(ENV_Type _type)
	: CScript(ENVSCRIPT)
	, m_bFirst(false)
	, m_bBreak(false)
	, m_eType(_type)
{
}

CENVScript::~CENVScript()
{
}

void CENVScript::tick()
{
	if (!m_bFirst)
	{
		m_bFirst = true;
		Animator2D()->Play(L"IDLE", true);
		if (m_eType == ENV_Type::lich)
		{
			CLevelMgr::GetInst()->playBGM(L"sound\\opening.mp3");
		}
	}
}

void CENVScript::BeginOverlap(CCollider2D* _pOther)
{
	if (!m_bBreak)
	{
		if (_pOther->GetOwner()->GetName() == L"Bullet" || _pOther->GetOwner()->GetName() == L"MonsterBullet")
		{
			_pOther->ReturnPool();
		}
		m_bBreak = true;
		Animator2D()->Play(L"BREAK", false);
		if (m_eType == ENV_Type::BOX)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\box_break_0" + to_wstring((rand() % 2) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
		}
		else if (m_eType == ENV_Type::BARREL)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\barrel_break_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.6f, true);
		}
	}
}

void CENVScript::EndOverlap(CCollider2D* _pOther)
{
	if (Collider2D()->getOverlapCount() == 0)
	{
		GetOwner()->DelComponent(COMPONENT_TYPE::COLLIDER2D);
	}
}

void CENVScript::SaveToFile(FILE* _File)
{
}

void CENVScript::LoadFromFile(FILE* _File)
{
}



