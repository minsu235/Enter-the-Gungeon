#include "pch.h"
#include "CResMgr.h"


CResMgr::CResMgr()
	: m_iLayoutOffset(0)
	, m_bChanged(false)
{

}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[i].begin();

		for (; iter != m_arrRes[i].end(); ++iter)
		{
			iter->second = nullptr;
		}
	}

	CSound::g_pFMOD->release();
}


Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);
	pTex->Create(_iWidth, _iHeight, _eFormat, _iBindFlag);

	AddRes<CTexture>(_strKey, pTex.Get());

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);
	pTex->Create(_Tex2D);

	AddRes<CTexture>(_strKey, pTex.Get());

	return pTex;
}

bool CResMgr::DeleteRes(RES_TYPE _Type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_Type].find(_strKey);

	if (m_arrRes[(UINT)_Type].end() != iter)
	{
		m_arrRes[(UINT)_Type].erase(iter);
		m_bChanged = true;
		return true;
	}

	return false;
}

void CResMgr::CreatePool()
{
	Ptr<CPrefab> pMonBulletPrefab = FindRes<CPrefab>(L"MonsterBullet");
	
	for (size_t i = 0; i < g_vecMonsterBullet.size(); ++i)
	{
		g_vecMonsterBullet[i] = pMonBulletPrefab->Instantiate();
	}
}