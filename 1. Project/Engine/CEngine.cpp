#include "pch.h"
#include "CEngine.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CDevice.h"
#include "CCollisionMgr.h"
#include "CFontMgr.h"
#include "CSound.h"





CEngine::CEngine()
	: m_hMainWnd(nullptr)	
{

}

CEngine::~CEngine()
{

}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hMainWnd = _hWnd;
	m_vResolution.x = (float)_iWidth;
	m_vResolution.y = (float)_iHeight;

	// 비트맵 해상도를 설정하기 위한 실제 윈도우 크기 계산
	RECT rt = { 0, 0, (LONG)_iWidth , (LONG)_iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	// 윈도우 크기 변경 및 Show 설정
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hMainWnd, true);

	// Manager 초기화
	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		return E_FAIL;
	}

	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
			
	CResMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();
	CFontMgr::GetInst()->init();

	return S_OK;
}

void CEngine::progress()
{
	tick();

	render();
}

void CEngine::tick()
{
	// Manager 업데이트
	CResMgr::GetInst()->tick();
	CSound::g_pFMOD->update();
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();
	CRenderMgr::GetInst()->tick();

	CLevelMgr::GetInst()->progress();	

	CCollisionMgr::GetInst()->tick();
}

void CEngine::render()
{	
	CDevice::GetInst()->TargetClear();

	CRenderMgr::GetInst()->render();
	CTimeMgr::GetInst()->render();

	CEventMgr::GetInst()->tick();
}
