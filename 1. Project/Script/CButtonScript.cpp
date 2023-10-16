#include "pch.h"
#include "CButtonScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CFontMgr.h>

CButtonScript::CButtonScript()
	: CScript(BUTTONSCRIPT)
	, m_bMouseOn(false)
{
}

CButtonScript::~CButtonScript()
{
}

void CButtonScript::tick()
{
	Vec3 vMousePos = CLevelMgr::GetInst()->getMousepos();
	Vec3 vButtonPos = Transform()->GetRelativePos();
	Vec3 vButtonScale = Transform()->GetRelativeScale();
	if (vButtonPos.x - vButtonScale.x / 2.f <= vMousePos.x && vMousePos.x <= vButtonPos.x + vButtonScale.x / 2.f
		&& vButtonPos.y - vButtonScale.y / 2.f <= vMousePos.y && vMousePos.y <= vButtonPos.y + vButtonScale.y / 2.f)
	{
		if (!m_bMouseOn)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\menu_select_01.wav");
			pSound->Play(1, 0.6f, true);
		}
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}

	if (m_bMouseOn)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\menu_confirm_01.wav");
			pSound->Play(1, 0.6f, true);
			CLevelMgr::GetInst()->LevelChange();
			CFontMgr::GetInst()->m_bBulletrender = true;
			CFontMgr::GetInst()->m_bGoldrender = true;
		}
	}
}

void CButtonScript::SaveToFile(FILE* _File)
{
}

void CButtonScript::LoadFromFile(FILE* _File)
{
}
