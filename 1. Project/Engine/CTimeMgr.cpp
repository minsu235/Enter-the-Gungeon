#include "pch.h"
#include "CTimeMgr.h"
#include "CEngine.h"


CTimeMgr::CTimeMgr()
	: m_llFrequence{}
	, m_llCurCount{}
	, m_llPrevCount{}
	, m_fAccTime(0.f)
	, m_fDeltaTime(0.f)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	// �ʴ� ������ 1000
	// GetTickCount();

	// �ʴ� ������ õ��
	QueryPerformanceFrequency(&m_llFrequence);

	// ���� ī����
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::tick()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequence.QuadPart;

	m_llPrevCount.QuadPart = m_llCurCount.QuadPart;

	// 60 �������� �ʰ��� ��� ���� ���
	if ((1.f / 60.f) < m_fDeltaTime)
	{
		m_fDeltaTime = (1.f / 60.f);
	}

	g_global.fDT = m_fDeltaTime;
	g_global.fAccTime += m_fDeltaTime;
}
#include "CFontMgr.h"

void CTimeMgr::render()
{
	static int iCount = 0;
	++iCount;
	static wchar_t szFloat[50] = {};

	// 1 �ʿ� �ѹ�
	m_fAccTime += m_fDeltaTime;
	if (1.f < m_fAccTime)
	{
		HWND hWnd = CEngine::GetInst()->GetMainHwnd();

		float FPS = 1.f / m_fDeltaTime;
		swprintf_s(szFloat, 50, L"DeltaTime : %d", iCount);
		int iLen = (int)wcsnlen_s(szFloat, 50);
		//TextOut(_dc, 10, 10, szFloat, iLen);

		SetWindowText(hWnd, szFloat);

		// �����ð�, ī��Ʈ �ʱ�ȭ
		m_fAccTime = 0.f;
		iCount = 0;
	}
}
