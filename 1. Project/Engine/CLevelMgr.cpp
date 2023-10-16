#include "pch.h"
#include "CLevelMgr.h"

#include "CResMgr.h"
#include "CCollisionMgr.h"
#include "CDevice.h"
#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CCamera.h"

#include "CLevel.h"
#include "CGameObject.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
	, m_pBGM(nullptr)
	, m_debug(false)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
	if (nullptr != m_pLevel)
		delete m_pLevel;
	Safe_Del_Vec(g_vecMonsterBullet);
}


LEVEL_STATE CLevelMgr::GetLevelState()
{ 
	return m_pCurLevel->GetState(); 
}

void CLevelMgr::playBGM(const wstring& _strKey)
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(_strKey);

	if (m_pBGM != nullptr && m_pBGM != pSound.Get())
	{
		m_pBGM->Stop();
	}
	m_pBGM = pSound.Get();

	pSound->Play(0, 0.2f, false);
}


void CLevelMgr::init()
{
	// Level 하나 제작하기
	m_pCurLevel = new CLevel;
	m_pCurLevel->SetName(L"Level 0");	
}

void CLevelMgr::progress()
{
	m_pCurLevel->ClearLayer();

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->tick();
	}
	
	m_pCurLevel->finaltick();

	{
		Vec2 vRes = CDevice::GetInst()->GetRenderResolution();
		Vec3 vMousePos = { CKeyMgr::GetInst()->GetMousePos().x, CKeyMgr::GetInst()->GetMousePos().y, 0.f };
		vMousePos.y = vRes.y - vMousePos.y;
		vMousePos.x -= vRes.x / 2;
		vMousePos.y -= vRes.y / 2;
		vMousePos.y /= vRes.y / 2;
		vMousePos.x /= vRes.x / 2;

		CCamera* maincam = CRenderMgr::GetInst()->GetMainCam();
		if (maincam != nullptr)
		{
			Matrix vCam = maincam->GetProjMat();
			vCam = XMMatrixInverse(nullptr, vCam);
			vMousePos = XMVector3TransformNormal(vMousePos, vCam);

			vCam = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
			vCam = XMMatrixInverse(nullptr, vCam);
			vMousePos = XMVector3TransformCoord(vMousePos, vCam);
			m_mousePos = vMousePos;
		}
	}

	if (KEY_TAP(KEY::SPACE))
	{
		m_debug = m_debug ^ 1;
	}

}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);
}

void CLevelMgr::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _out)
{
	m_pCurLevel->FindObjectByName(_strName, _out);
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (m_pCurLevel)
	{
		delete m_pCurLevel;
	}

	m_pCurLevel = _NextLevel;
}

void CLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
	assert(!(m_pCurLevel->GetState() == _State));

	m_pCurLevel->SetState(_State);

	if(LEVEL_STATE::PLAY == _State)
		m_pCurLevel->begin();
}
