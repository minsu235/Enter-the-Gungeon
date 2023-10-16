#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMaterial.h>
#include <Engine\CLevelMgr.h>
#include <Engine/CResMgr.h>
#include <Engine\CRenderMgr.h>

#include "CGunScript.h"
#include "CGameCameraScript.h"

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_fAccTime(0.f)
	, m_fSpeed(400.f)
	, m_eCurrentState(PLAYER_STATE::SPIN)
	, m_iHP(6)
	, m_iBlankCount(2)
	, m_UI(nullptr)
	, m_bInvi(false)
	, m_fInvitime(0.f)
	, m_fRuntime(0.f)
	, m_iRunCount(0)
	, m_bFall(false)
	, m_iCurrentGun(1)
	, m_fSpread(1.f)
	, m_bFly(false)
	, m_iGold(0)
	, m_bStop(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player MoveSpeed",  &m_fSpeed);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
}

void CPlayerScript::tick()
{
	if (KEY_TAP(KEY::G))
	{
		addGold(10);
	}
	if (KEY_TAP(KEY::H))
	{
		addHP(2);
	}
	if (KEY_TAP(KEY::F))
	{
		addBlank();
	}

	if (m_bStop)
		return;

	Vec3 vPos = Transform()->GetRelativePos();
	if (m_eCurrentState == PLAYER_STATE::SPIN)
	{
		if (m_fAccTime == 0.f)
		{
			Animator2D()->Play(L"SPIN", true);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
			pSound->Play(1, 0.6f, true);
			CLevelMgr::GetInst()->playBGM(L"sound\\stage1bgm.mp3");
		}
		m_fAccTime += DT;
		vPos.y -= 400.f * DT;
		Transform()->SetRelativePos(vPos);
		if (m_fAccTime >= 1.f)
		{
			m_eCurrentState = PLAYER_STATE::IDLE_F;
			m_fAccTime = 0.f;
		}
		return;
	}
	if (KEY_TAP(KEY::Q))
	{
		if (m_iBlankCount != 0)
		{
			m_iBlankCount -= 1;
			m_UI->GetScript<CUIScript>()->setBlank(m_iBlankCount);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\silenceblank_use_01.wav");
			pSound->Play(1, 0.6f, true);
			CGameObject* Blank = CResMgr::GetInst()->FindRes<CPrefab>(L"Blank")->Instantiate();
			Instantiate(Blank, vPos - Vec3(0.f, 30.f, 0.f), 9);
			CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"blankeffect")->Instantiate();
			impact->Animator2D()->Play(L"blankeffect", false);
			Instantiate(impact, vPos - Vec3(0.f, 30.f, 0.f), 0);
		}
	}
	if (KEY_TAP(KEY::RBTN))
	{
		if (m_eCurrentState == PLAYER_STATE::RUN_B)
		{
			m_eCurrentState = PLAYER_STATE::DODGE_B;
			Animator2D()->Play(L"Dodge_B", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_leap_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.4f, true);
			m_fRuntime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_eCurrentState == PLAYER_STATE::RUN_F)
		{
			m_eCurrentState = PLAYER_STATE::DODGE_F;
			Animator2D()->Play(L"Dodge_F", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_leap_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.4f, true);
			m_fRuntime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_eCurrentState == PLAYER_STATE::RUN_R)
		{
			m_eCurrentState = PLAYER_STATE::DODGE_R;
			Animator2D()->Play(L"Dodge_R", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_leap_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.4f, true);
			m_fRuntime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_eCurrentState == PLAYER_STATE::RUN_L)
		{
			m_eCurrentState = PLAYER_STATE::DODGE_L;
			Animator2D()->Play(L"Dodge_L", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_leap_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.4f, true);
			m_fRuntime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_eCurrentState == PLAYER_STATE::RUN_FR)
		{
			m_eCurrentState = PLAYER_STATE::DODGE_FR;
			Animator2D()->Play(L"Dodge_R", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_leap_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.4f, true);
			m_fRuntime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_eCurrentState == PLAYER_STATE::RUN_FL)
		{
			m_eCurrentState = PLAYER_STATE::DODGE_FL;
			Animator2D()->Play(L"Dodge_L", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_leap_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.4f, true);
			m_fRuntime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_eCurrentState == PLAYER_STATE::RUN_BR)
		{
			m_eCurrentState = PLAYER_STATE::DODGE_BR;
			Animator2D()->Play(L"Dodge_BR", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_leap_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.4f, true);
			m_fRuntime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_eCurrentState == PLAYER_STATE::RUN_BL)
		{
			m_eCurrentState = PLAYER_STATE::DODGE_BL;
			Animator2D()->Play(L"Dodge_BL", false);
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_leap_0" + to_wstring((rand() % 3) + 1) + L".wav");
			pSound->Play(1, 0.4f, true);
			m_fRuntime = 0.f;
			m_iRunCount = 0;
		}
	}
	if (KEY_TAP(KEY::N_1))
	{
		if (m_iCurrentGun != 1)
		{
			if (GetOwner()->GetChildObject()[0]->GetChildObject().size() >= 1)
			{
				GetOwner()->GetChildObject()[0]->GetChildObject()[m_iCurrentGun - 1]->GetScript<CGunScript>()->setDeact();
				GetOwner()->GetChildObject()[0]->GetChildObject()[0]->GetScript<CGunScript>()->setAct();
				m_iCurrentGun = 1;
			}
		}
	}
	if (KEY_TAP(KEY::N_2))
	{
		if (m_iCurrentGun != 2)
		{
			if (GetOwner()->GetChildObject()[0]->GetChildObject().size() >= 2)
			{
				GetOwner()->GetChildObject()[0]->GetChildObject()[m_iCurrentGun - 1]->GetScript<CGunScript>()->setDeact();
				GetOwner()->GetChildObject()[0]->GetChildObject()[1]->GetScript<CGunScript>()->setAct();
				m_iCurrentGun = 2;
			}
		}
	}
	if (KEY_TAP(KEY::N_3))
	{
		if (m_iCurrentGun != 3)
		{
			if (GetOwner()->GetChildObject()[0]->GetChildObject().size() >= 3)
			{
				GetOwner()->GetChildObject()[0]->GetChildObject()[m_iCurrentGun - 1]->GetScript<CGunScript>()->setDeact();
				GetOwner()->GetChildObject()[0]->GetChildObject()[2]->GetScript<CGunScript>()->setAct();
				m_iCurrentGun = 3;
			}
		}
	}
	if (m_eCurrentState == PLAYER_STATE::DODGE_B)
	{
		m_fAccTime += DT;
		vPos.y += 640.f * DT;

		if (m_fAccTime > 0.45f)
		{
			m_eCurrentState = PLAYER_STATE::IDLE_B;
			playIdle();
			m_fAccTime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_fAccTime > 0.3f)
		{
			if (m_bFall)
			{
				m_eCurrentState = PLAYER_STATE::DODGEFALL_B;
				Animator2D()->Play(L"FALL_Dodge", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
				pSound->Play(1, 0.6f, true);
				m_fAccTime = 0.f;
				m_bInvi = false;
				m_fInvitime = 0.f;
				vPos.z = 75.f;
			}
			else if (m_iRunCount == 0)
			{
				if (!m_bFly)
				{
					CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_dodge")->Instantiate();
					impact->Animator2D()->Play(L"dustup_dodge", false);
					Vec3 vPos = Transform()->GetRelativePos();
					vPos.z = 80.f;
					vPos.y -= 45.f;
					Instantiate(impact, vPos, 0);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_roll_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.4f, true);
				}
				++m_iRunCount;
			}
		}
	}
	else if (m_eCurrentState == PLAYER_STATE::DODGE_F)
	{
		m_fAccTime += DT;
		vPos.y -= 640.f * DT;
		if (m_fAccTime > 0.45f)
		{
			m_eCurrentState = PLAYER_STATE::IDLE_F;
			playIdle();
			m_fAccTime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_fAccTime > 0.3f)
		{
			if (m_bFall)
			{
				m_eCurrentState = PLAYER_STATE::DODGEFALL_F;
				Animator2D()->Play(L"FALL_Dodge", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
				pSound->Play(1, 0.6f, true);
				m_fAccTime = 0.f;
				m_bInvi = false;
				m_fInvitime = 0.f;
				vPos.z = 75.f;
			}
			else if (m_iRunCount == 0)
			{
				if (!m_bFly)
				{
					CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_dodge")->Instantiate();
					impact->Animator2D()->Play(L"dustup_dodge", false);
					Vec3 vPos = Transform()->GetRelativePos();
					vPos.z = 80.f;
					vPos.y -= 45.f;
					Instantiate(impact, vPos, 0);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_roll_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.4f, true);
				}
				++m_iRunCount;
			}
		}
	}
	else if (m_eCurrentState == PLAYER_STATE::DODGE_L)
	{
		m_fAccTime += DT;
		vPos.x -= 640.f * DT;
		if (m_fAccTime > 0.45f)
		{
			m_eCurrentState = PLAYER_STATE::IDLE_L;
			playIdle();
			m_fAccTime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_fAccTime > 0.3f)
		{
			if (m_bFall)
			{
				m_eCurrentState = PLAYER_STATE::DODGEFALL_L;
				Animator2D()->Play(L"FALL_Dodge", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
				pSound->Play(1, 0.6f, true);
				m_fAccTime = 0.f;
				m_bInvi = false;
				m_fInvitime = 0.f;
				vPos.z = 75.f;
			}
			else if (m_iRunCount == 0)
			{
				if (!m_bFly)
				{
					CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_dodge")->Instantiate();
					impact->Animator2D()->Play(L"dustup_dodge", false);
					Vec3 vPos = Transform()->GetRelativePos();
					vPos.z = 80.f;
					vPos.y -= 45.f;
					Instantiate(impact, vPos, 0);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_roll_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.4f, true);
				}
				++m_iRunCount;
			}
		}
	}
	else if (m_eCurrentState == PLAYER_STATE::DODGE_R)
	{
		m_fAccTime += DT;
		vPos.x += 640.f * DT;

		if (m_fAccTime > 0.45f)
		{
			m_eCurrentState = PLAYER_STATE::IDLE_R;
			playIdle();
			m_fAccTime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_fAccTime > 0.3f)
		{
			if (m_bFall)
			{
				m_eCurrentState = PLAYER_STATE::DODGEFALL_R;
				Animator2D()->Play(L"FALL_Dodge", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
				pSound->Play(1, 0.6f, true);
				m_fAccTime = 0.f;
				m_bInvi = false;
				m_fInvitime = 0.f;
				vPos.z = 75.f;
			}
			else if (m_iRunCount == 0)
			{
				if (!m_bFly)
				{
					CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_dodge")->Instantiate();
					impact->Animator2D()->Play(L"dustup_dodge", false);
					Vec3 vPos = Transform()->GetRelativePos();
					vPos.z = 80.f;
					vPos.y -= 45.f;
					Instantiate(impact, vPos, 0);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_roll_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.4f, true);
				}
				++m_iRunCount;
			}
		}
	}
	else if (m_eCurrentState == PLAYER_STATE::DODGE_FR)
	{
		m_fAccTime += DT;
		vPos.x += 640.f / (float)sqrt(2) * DT;
		vPos.y -= 640.f / (float)sqrt(2) * DT;

		if (m_fAccTime > 0.45f)
		{
			m_eCurrentState = PLAYER_STATE::IDLE_FR;
			playIdle();
			m_fAccTime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_fAccTime > 0.3f)
		{
			if (m_bFall)
			{
				m_eCurrentState = PLAYER_STATE::DODGEFALL_FR;
				Animator2D()->Play(L"FALL_Dodge", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
				pSound->Play(1, 0.6f, true);
				m_fAccTime = 0.f;
				m_bInvi = false;
				m_fInvitime = 0.f;
				vPos.z = 75.f;
			}
			else if (m_iRunCount == 0)
			{
				if (!m_bFly)
				{
					CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_dodge")->Instantiate();
					impact->Animator2D()->Play(L"dustup_dodge", false);
					Vec3 vPos = Transform()->GetRelativePos();
					vPos.z = 80.f;
					vPos.y -= 45.f;
					Instantiate(impact, vPos, 0);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_roll_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.4f, true);
				}
				++m_iRunCount;
			}
		}
	}
	else if (m_eCurrentState == PLAYER_STATE::DODGE_FL)
	{
		m_fAccTime += DT;
		vPos.x -= 640.f / (float)sqrt(2) * DT;
		vPos.y -= 640.f / (float)sqrt(2) * DT;

		if (m_fAccTime > 0.45f)
		{
			m_eCurrentState = PLAYER_STATE::IDLE_FL;
			playIdle();
			m_fAccTime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_fAccTime > 0.3f)
		{
			if (m_bFall)
			{
				m_eCurrentState = PLAYER_STATE::DODGEFALL_FL;
				Animator2D()->Play(L"FALL_Dodge", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
				pSound->Play(1, 0.6f, true);
				m_fAccTime = 0.f;
				m_bInvi = false;
				m_fInvitime = 0.f;
				vPos.z = 75.f;
			}
			else if (m_iRunCount == 0)
			{
				if (!m_bFly)
				{
					CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_dodge")->Instantiate();
					impact->Animator2D()->Play(L"dustup_dodge", false);
					Vec3 vPos = Transform()->GetRelativePos();
					vPos.z = 80.f;
					vPos.y -= 45.f;
					Instantiate(impact, vPos, 0);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_roll_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.4f, true);
				}
				++m_iRunCount;
			}
		}
	}
	else if (m_eCurrentState == PLAYER_STATE::DODGE_BR)
	{
		m_fAccTime += DT;
		vPos.x += 640.f / (float)sqrt(2) * DT;
		vPos.y += 640.f / (float)sqrt(2) * DT;

		if (m_fAccTime > 0.45f)
		{
			m_eCurrentState = PLAYER_STATE::IDLE_BR;
			playIdle();
			m_fAccTime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_fAccTime > 0.3f)
		{
			if (m_bFall)
			{
				m_eCurrentState = PLAYER_STATE::DODGEFALL_BR;
				Animator2D()->Play(L"FALL_Dodge", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
				pSound->Play(1, 0.6f, true);
				m_fAccTime = 0.f;
				m_bInvi = false;
				m_fInvitime = 0.f;
				vPos.z = 75.f;
			}
			else if (m_iRunCount == 0)
			{
				if (!m_bFly)
				{
					CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_dodge")->Instantiate();
					impact->Animator2D()->Play(L"dustup_dodge", false);
					Vec3 vPos = Transform()->GetRelativePos();
					vPos.z = 80.f;
					vPos.y -= 45.f;
					Instantiate(impact, vPos, 0);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_roll_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.4f, true);
				}
				++m_iRunCount;
			}
		}
	}
	else if (m_eCurrentState == PLAYER_STATE::DODGE_BL)
	{
		m_fAccTime += DT;
		vPos.x -= 640.f / (float)sqrt(2) * DT;
		vPos.y += 640.f / (float)sqrt(2) * DT;

		if (m_fAccTime > 0.45f)
		{
			m_eCurrentState = PLAYER_STATE::IDLE_BL;
			playIdle();
			m_fAccTime = 0.f;
			m_iRunCount = 0;
		}
		else if (m_fAccTime > 0.3f)
		{
			if (m_bFall)
			{
				m_eCurrentState = PLAYER_STATE::DODGEFALL_BL;
				Animator2D()->Play(L"FALL_Dodge", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
				pSound->Play(1, 0.6f, true);
				m_fAccTime = 0.f;
				m_bInvi = false;
				m_fInvitime = 0.f;
				vPos.z = 75.f;
			}
			else if(m_iRunCount == 0)
			{
				if (!m_bFly)
				{
					CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_dodge")->Instantiate();
					impact->Animator2D()->Play(L"dustup_dodge", false);
					Vec3 vPos = Transform()->GetRelativePos();
					vPos.z = 80.f;
					vPos.y -= 45.f;
					Instantiate(impact, vPos, 0);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dodge_roll_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.4f, true);
				}
				++m_iRunCount;
			}
		}
	}
	else if (m_eCurrentState >= PLAYER_STATE::FALL_B)
	{
		m_fAccTime += DT;
		if (m_fAccTime > 2.f)
		{
			switch (m_eCurrentState)
			{
			case PLAYER_STATE::FALL_B:
				vPos.y -= 20.f;
				break;
			case PLAYER_STATE::FALL_F:
				vPos.y += 120.f;
				break;
			case PLAYER_STATE::FALL_R:
				vPos.y += 80.f;
				vPos.x -= 60.f;
				break;
			case PLAYER_STATE::FALL_L:
				vPos.y += 80.f;
				vPos.x += 60.f;
				break;
			}
			vPos.z = 75.f;
			m_eCurrentState = PLAYER_STATE::IDLE_F;
			m_fAccTime = 0.f;
			if (m_iHP != 1)
			{
				--m_iHP;
				m_UI->GetScript<CUIScript>()->setHP(m_iHP);
			}
			m_bInvi = true;
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\general_hurt_01.wav");
			pSound->Play(1, 0.6f, true);
			CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->camerashake();
			pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\respawn.wav");
			pSound->Play(1, 0.6f, true);
		}
		else if (m_fAccTime < 0.4f)
		{
			vPos.y -= 200.f * DT;
		}
	}
	else if (m_eCurrentState >= PLAYER_STATE::DODGEFALL_B)
	{
		m_fAccTime += DT;
		if (m_fAccTime > 2.f)
		{
			vPos.y += 80.f;
			switch (m_eCurrentState)
			{
			case PLAYER_STATE::DODGEFALL_B:
				vPos.y -= 192.f;
				break;
			case PLAYER_STATE::DODGEFALL_F:
				vPos.y += 192.f;
				break;
			case PLAYER_STATE::DODGEFALL_R:
				vPos.x -= 192.f;
				break;
			case PLAYER_STATE::DODGEFALL_L:
				vPos.x += 192.f;
				break;
			case PLAYER_STATE::DODGEFALL_FR:
				vPos.y += 192.f / 1.414f;
				vPos.x -= 192.f / 1.414f;
				break;
			case PLAYER_STATE::DODGEFALL_FL:
				vPos.y += 192.f / 1.414f;
				vPos.x += 192.f / 1.414f;
				break;
			case PLAYER_STATE::DODGEFALL_BR:
				vPos.y -= 192.f / 1.414f;
				vPos.x -= 192.f / 1.414f;
				break;
			case PLAYER_STATE::DODGEFALL_BL:
				vPos.y -= 192.f / 1.414f;
				vPos.x += 192.f / 1.414f;
				break;
			}
			vPos.z = 75.f;
			m_eCurrentState = PLAYER_STATE::IDLE_F;
			m_fAccTime = 0.f;
			if (m_iHP != 1)
			{
				--m_iHP;
				m_UI->GetScript<CUIScript>()->setHP(m_iHP);
			}
			m_bInvi = true;
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\general_hurt_01.wav");
			pSound->Play(1, 0.6f, true);
			CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->camerashake();
			pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\respawn.wav");
			pSound->Play(1, 0.6f, true);
		}
		else if (m_fAccTime < 0.4f)
		{
			vPos.y -= 200.f * DT;
		}
	}
	else if (KEY_PRESSED(KEY::A))
	{
		if (KEY_PRESSED(KEY::S))
		{
			vPos.x -= m_fSpeed / (float)sqrt(2) * DT;
			vPos.y -= m_fSpeed / (float)sqrt(2) * DT;
			if(!m_bFly)
				playRun();
			else
				playIdle();
			m_eCurrentState = PLAYER_STATE::RUN_FL;
		}
		else if (KEY_PRESSED(KEY::W))
		{
			vPos.x -= m_fSpeed / (float)sqrt(2) * DT;
			vPos.y += m_fSpeed / (float)sqrt(2) * DT;
			if (!m_bFly)
				playRun();
			else
				playIdle();
			m_eCurrentState = PLAYER_STATE::RUN_BL;
		}
		else
		{
			vPos.x -= m_fSpeed * DT;
			if (!m_bFly)
				playRun();
			else
				playIdle();
			m_eCurrentState = PLAYER_STATE::RUN_L;
		}
	}
	else if (KEY_PRESSED(KEY::D))
	{
		if (KEY_PRESSED(KEY::S))
		{
			vPos.x += m_fSpeed / (float)sqrt(2) * DT;
			vPos.y -= m_fSpeed / (float)sqrt(2) * DT;
			if (!m_bFly)
				playRun();
			else
				playIdle();
			m_eCurrentState = PLAYER_STATE::RUN_FR;
		}
		else if (KEY_PRESSED(KEY::W))
		{
			vPos.x += m_fSpeed / (float)sqrt(2) * DT;
			vPos.y += m_fSpeed / (float)sqrt(2) * DT;
			if (!m_bFly)
				playRun();
			else
				playIdle();
			m_eCurrentState = PLAYER_STATE::RUN_BR;
		}
		else
		{
			vPos.x += m_fSpeed * DT;
			if (!m_bFly)
				playRun();
			else
				playIdle();
			m_eCurrentState = PLAYER_STATE::RUN_R;
		}
	}
	else if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= m_fSpeed * DT;
		if (!m_bFly)
			playRun();
		else
			playIdle();
		m_eCurrentState = PLAYER_STATE::RUN_F;
	}
	else if (KEY_PRESSED(KEY::W))
	{
		vPos.y += m_fSpeed * DT;
		if (!m_bFly)
			playRun();
		else
			playIdle();
		m_eCurrentState = PLAYER_STATE::RUN_B;
	}
	else
	{
		playIdle();
	}

	/*if (KEY_PRESSED(KEY::Y))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.z += DT * XM_PI;
		Transform()->SetRelativeRotation(vRot);

		float fZRot = Collider2D()->GetRotationZ();
		fZRot += DT * XM_PI;
		Collider2D()->SetRotationZ(fZRot);
	}*/

	if (m_bInvi)
	{
		m_fInvitime += DT;
		if (m_fInvitime > 1.f)
		{
			m_bInvi = false;
			m_fInvitime = 0.f;
			vPos.z = 75.f;
		}
		else if (m_fInvitime > 0.9375f)
		{
			vPos.z = 75.f;
		}
		else if (m_fInvitime > 0.875f)
		{
			vPos.z = 1001.f;
		}
		else if (m_fInvitime > 0.8125f)
		{
			vPos.z = 75.f;
		}
		else if (m_fInvitime > 0.75f)
		{
			vPos.z = 1001.f;
		}
		else if (m_fInvitime > 0.6875f)
		{
			vPos.z = 75.f;
		}
		else if (m_fInvitime > 0.625f)
		{
			vPos.z = 1001.f;
		}
		else if (m_fInvitime > 0.5625f)
		{
			vPos.z = 75.f;
		}
		else if (m_fInvitime > 0.5f)
		{
			vPos.z = 1001.f;
		}
		else if (m_fInvitime > 0.4375f)
		{
			vPos.z = 75.f;
		}
		else if (m_fInvitime > 0.375f)
		{
			vPos.z = 1001.f;
		}
		else if (m_fInvitime > 0.3125f)
		{
			vPos.z = 75.f;
		}
		else if (m_fInvitime > 0.25f)
		{
			vPos.z = 1001.f;
		}
		else if (m_fInvitime > 0.1875f)
		{
			vPos.z = 75.f;
		}
		else if (m_fInvitime > 0.125f)
		{
			vPos.z = 1001.f;
		}
		else if (m_fInvitime > 0.0625f)
		{
			vPos.z = 75.f;
		}
		else if (m_fInvitime > 0.f)
		{
			vPos.z = 1001.f;
		}
	}
	
	if(vPos != Transform()->GetRelativePos())
		Transform()->SetRelativePos(vPos);
}

MOUSE_DIR CPlayerScript::getDIR()
{
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	vPlayerPos.y -= 20.f;

	Vec3 vMousePos = CLevelMgr::GetInst()->getMousepos();
	

	float degree = atan2((vMousePos.y - vPlayerPos.y), (vMousePos.x - vPlayerPos.x)) * 180 / 3.141592f;
	if (-22.5 <= degree && degree < 22.5)
		return MOUSE_DIR::R;
	else if (22.5 <= degree && degree < 67.5)
		return MOUSE_DIR::UR;
	else if (67.5 <= degree && degree < 112.5)
		return MOUSE_DIR::U;
	else if (112.5 <= degree && degree < 157.5)
		return MOUSE_DIR::UL;
	else if (-67.5 <= degree && degree < -22.5)
		return MOUSE_DIR::DR;
	else if (-112.5 <= degree && degree < -67.5)
		return MOUSE_DIR::D;
	else if (-157.5 <= degree && degree < -112.5)
		return MOUSE_DIR::DL;
	else
		return MOUSE_DIR::L;
}

void CPlayerScript::playRun()
{
	m_fRuntime += DT;
	if (m_fRuntime > 0.325f && m_iRunCount == 1)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\boot_carpet_0" + to_wstring((rand() % 3) + 1) + L".wav");
		pSound->Play(1, 0.4f, true);
		CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_run")->Instantiate();
		impact->Animator2D()->Play(L"dustup_run", false);
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.z = 80.f;
		vPos.y -= 45.f;
		Instantiate(impact, vPos, 0);
		m_fRuntime = 0.f;
		m_iRunCount = 0;
	}
	else if (m_fRuntime > 0.1625f && m_iRunCount == 0)
	{
		CGameObject* impact = CResMgr::GetInst()->FindRes<CPrefab>(L"dustup_run")->Instantiate();
		impact->Animator2D()->Play(L"dustup_run", false);
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.z = 80.f;
		vPos.y -= 45.f;
		Instantiate(impact, vPos, 0);
		++m_iRunCount;
	}

	MOUSE_DIR dir = getDIR();
	switch (dir)
	{
	case MOUSE_DIR::U:
		Animator2D()->Play(L"RUN_B", true);
		break;
	case MOUSE_DIR::UR:
		Animator2D()->Play(L"RUN_BR", true);
		break;
	case MOUSE_DIR::UL:
		Animator2D()->Play(L"RUN_BL", true);
		break;
	case MOUSE_DIR::D:
		Animator2D()->Play(L"RUN_F", true);
		break;
	case MOUSE_DIR::DR:
		Animator2D()->Play(L"RUN_R", true);
		break;
	case MOUSE_DIR::DL:
		Animator2D()->Play(L"RUN_L", true);
		break;
	case MOUSE_DIR::R:
		Animator2D()->Play(L"RUN_R", true);
		break;
	case MOUSE_DIR::L:
		Animator2D()->Play(L"RUN_L", true);
		break;
	}
}

void CPlayerScript::playIdle()
{
	m_fRuntime = 0.f;
	m_iRunCount = 0;

	MOUSE_DIR dir = getDIR();
	switch (dir)
	{
	case MOUSE_DIR::U:
		m_eCurrentState = PLAYER_STATE::IDLE_B;
		Animator2D()->Play(L"IDLE_B", true);
		break;
	case MOUSE_DIR::UR:
		m_eCurrentState = PLAYER_STATE::IDLE_BR;
		Animator2D()->Play(L"IDLE_BR", true);
		break;
	case MOUSE_DIR::UL:
		m_eCurrentState = PLAYER_STATE::IDLE_BL;
		Animator2D()->Play(L"IDLE_BL", true);
		break;
	case MOUSE_DIR::D:
		m_eCurrentState = PLAYER_STATE::IDLE_F;
		Animator2D()->Play(L"IDLE_F", true);
		break;
	case MOUSE_DIR::DR:
		m_eCurrentState = PLAYER_STATE::IDLE_FR;
		Animator2D()->Play(L"IDLE_R", true);
		break;
	case MOUSE_DIR::DL:
		m_eCurrentState = PLAYER_STATE::IDLE_FL;
		Animator2D()->Play(L"IDLE_L", true);
		break;
	case MOUSE_DIR::R:
		m_eCurrentState = PLAYER_STATE::IDLE_R;
		Animator2D()->Play(L"IDLE_R", true);
		break;
	case MOUSE_DIR::L:
		m_eCurrentState = PLAYER_STATE::IDLE_L;
		Animator2D()->Play(L"IDLE_L", true);
		break;
	}
}

void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"MonsterBullet" && m_eCurrentState < PLAYER_STATE::DODGEFALL_B && m_eCurrentState > PLAYER_STATE::DODGE_BL)
	{
		if (!m_bInvi)
		{
			_pOther->ReturnPool();
			CGameObject* bullet_impact = CResMgr::GetInst()->FindRes<CPrefab>(L"bullet_impact")->Instantiate();
			bullet_impact->Animator2D()->Play(L"bullet_impact", false);
			Instantiate(bullet_impact, _pOther->Transform()->GetRelativePos(), 0);
			if (m_iHP != 1)
			{
				--m_iHP;
				m_UI->GetScript<CUIScript>()->setHP(m_iHP);
			}
			m_bInvi = true;
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\general_hurt_01.wav");
			pSound->Play(1, 0.6f, true);
			CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->camerashake();
		}
	}
	if (_pOther->GetOwner()->GetName() == L"Cliff")
	{
		if (!m_bFly)
		{
			if (m_eCurrentState >= PLAYER_STATE::RUN_B && m_eCurrentState <= PLAYER_STATE::RUN_BL)
			{
				Animator2D()->Play(L"FALL", false);
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\pit_fall.wav");
				pSound->Play(1, 0.6f, true);
				m_fAccTime = 0.f;
				m_fRuntime = 0.f;
				m_iRunCount = 0;
				m_bInvi = false;
				m_fInvitime = 0.f;
				m_bFall = false;
				Vec3 vPos = Transform()->GetRelativePos();
				vPos.z = 75.f;
				Transform()->SetRelativePos(vPos);

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
					m_eCurrentState = PLAYER_STATE::FALL_F;
				}
				if (down)
				{
					m_eCurrentState = PLAYER_STATE::FALL_B;
				}
				if (left)
				{
					m_eCurrentState = PLAYER_STATE::FALL_R;
				}
				if (right)
				{
					m_eCurrentState = PLAYER_STATE::FALL_L;
				}
			}
			else if (m_eCurrentState <= PLAYER_STATE::DODGE_BL)
			{
				m_bFall = true;
			}
		}
	}
}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
	if (m_eCurrentState == PLAYER_STATE::SPIN)
		return;
	if (_pOther->GetOwner()->GetName() == L"Monster" && m_eCurrentState < PLAYER_STATE::DODGEFALL_B && m_eCurrentState > PLAYER_STATE::DODGE_BL)
	{
		if (!m_bInvi)
		{
			if (m_iHP != 1)
			{
				--m_iHP;
				m_UI->GetScript<CUIScript>()->setHP(m_iHP);
			}
			m_bInvi = true;
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\general_hurt_01.wav");
			pSound->Play(1, 0.6f, true);
			CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->camerashake();
		}
	}
	if (_pOther->GetOwner()->GetName() == L"Wall")
	{
		bool left = false, right = false, up = false, down = false;
		Vec2 Playerpos = Collider2D()->GetFinalPos();
		Vec2 Playerscale = Collider2D()->GetScale();
		
		Vec2 Otherpos = Vec2(_pOther->GetOwner()->GetParent()->Transform()->GetRelativePos().x + _pOther->GetOwner()->Transform()->GetRelativePos().x, 
								_pOther->GetOwner()->GetParent()->Transform()->GetRelativePos().y +_pOther->GetOwner()->Transform()->GetRelativePos().y);
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
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Cliff")
	{
		m_bFall = false;
		if (m_eCurrentState >= PLAYER_STATE::DODGEFALL_B)
		{
			Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(0.f,0.f,1000.f));
		}
	}
}


void CPlayerScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
	fread(&m_fSpeed, sizeof(float), 1, _File);
}
