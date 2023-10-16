#include "pch.h"
#include "CFontMgr.h"


#include "CDevice.h"


CFontMgr::CFontMgr()
	: m_pFW1Factory(nullptr)
	, m_pFontWrapper(nullptr)
	, m_bGoldrender(false)
	, m_bBulletrender(false)
	, m_iGold(0)
	, m_iBullet(0)
	, m_iMaxBullet(0)
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_pFW1Factory)
		m_pFW1Factory->Release();

	if (nullptr != m_pFontWrapper)
		m_pFontWrapper->Release();
}

void CFontMgr::init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Romulus", &m_pFontWrapper)))
	{
		assert(NULL);
	}
}

void CFontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_pFontWrapper->DrawString(
		CONTEXT,
		_pStr, // String
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::render()
{
	if (m_bGoldrender)
	{
		wchar_t szfloat[10] = {};
		swprintf_s(szfloat, 10, L"%d", m_iGold);
		CFontMgr::GetInst()->DrawFont(szfloat, 55.f, 95.f, 35, FONT_RGBA(255, 255, 255, 255));
	}
	if (m_bBulletrender)
	{
		//¡Ä
		if (m_iMaxBullet == 0)
		{
			CFontMgr::GetInst()->DrawFont(L"¡Ä", 1355.f, 655.f, 50, FONT_RGBA(255, 255, 255, 255));
		}
		else
		{
			wchar_t szfloat[10] = {};
			swprintf_s(szfloat, 10, L"%d/%d", m_iBullet, m_iMaxBullet);
			CFontMgr::GetInst()->DrawFont(szfloat, 1290.f, 675.f, 35, FONT_RGBA(255, 255, 255, 255));
		}
	}
}
