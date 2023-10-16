#include "pch.h"
#include "CWingScript.h"
#include "CPlayerScript.h"

CWingScript::CWingScript()
	: CScript(WINGSCRIPT)
	, m_pPlayer(nullptr)
{
}

CWingScript::~CWingScript()
{
}

void CWingScript::tick()
{
	if (nullptr != m_pPlayer)
	{
		if (PLAYER_STATE::DODGE_BL >= m_pPlayer->GetScript<CPlayerScript>()->getPlayerState())
		{
			Animator2D()->Play(L"X", false);
		}
		else
		{
			Animator2D()->Play(L"Fly", true);
		}
		Vec3 vPos = m_pPlayer->Transform()->GetRelativePos();
		vPos.y -= 30.f;
		if (MOUSE_DIR::U == m_pPlayer->GetScript<CPlayerScript>()->getDIR()
			|| MOUSE_DIR::UL == m_pPlayer->GetScript<CPlayerScript>()->getDIR()
			|| MOUSE_DIR::UR == m_pPlayer->GetScript<CPlayerScript>()->getDIR())
		{
			vPos.z = 74.f;
		}
		else
		{
			vPos.z = 76.f;
		}
		Transform()->SetRelativePos(vPos);
	}
}

void CWingScript::SaveToFile(FILE* _File)
{
}

void CWingScript::LoadFromFile(FILE* _File)
{
}


