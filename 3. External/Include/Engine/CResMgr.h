#pragma once


#include "CPathMgr.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CMaterial.h"
#include "CPrefab.h"
#include "CSound.h"

extern vector<CGameObject*> g_vecMonsterBullet;

class CResMgr
	: public CSingleton<CResMgr>
{	
private:
	map<wstring, Ptr<CRes>>				m_arrRes[(UINT)RES_TYPE::END];	
	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecLayoutInfo;
	UINT								m_iLayoutOffset;

	bool								m_bChanged;

	vector<wstring>						m_vecContentName;

public:	
	void tick() { m_bChanged = false; }

	bool IsChanged() { return m_bChanged; }

	template<typename T>
	void AddRes(const wstring& _strKey, T* _pRes);

	template<typename T>
	Ptr<T> FindRes(const wstring& _strKey);

	template<typename T>
	Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);

	template<typename T>
	wstring GetNewResName();
		

	Ptr<CTexture> CreateTexture(const wstring& _strKey
		, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag );
	Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D);
	void CreatePool();
		
	const map<wstring, Ptr<CRes>>& GetResource(RES_TYPE _eResType) { return m_arrRes[(UINT)_eResType]; }

public:
	void init();

	const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutInfo() { return m_vecLayoutInfo; }

private:
	void InitSound();
	void CreateDefaultMesh();
	void CreateDefaultTexture();
	void CreateDefaultGraphicsShader();
	void CreateDefaultComputeShader();
	void CreateDefaultMaterial();

	void LoadContent();
	void FindContentFileName(const wstring& _strFolderPath);
	RES_TYPE GetResTypeByExt(wstring _filename);

	void AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName);

	bool DeleteRes(RES_TYPE _Type, const wstring& _strKey);

public:
	CResMgr();
	~CResMgr();

	friend class CEventMgr;
};

template<typename T>
RES_TYPE GetType()
{
	if (typeid(T).hash_code() == typeid(CMesh).hash_code())
		return RES_TYPE::MESH;

	if (typeid(T).hash_code() == typeid(CTexture).hash_code())
		return RES_TYPE::TEXTURE;

	if (typeid(T).hash_code() == typeid(CGraphicsShader).hash_code())
		return RES_TYPE::GRAPHICS_SHADER;

	if (typeid(T).hash_code() == typeid(CComputeShader).hash_code())
		return RES_TYPE::COMPUTE_SHADER;

	if (typeid(T).hash_code() == typeid(CMaterial).hash_code())
		return RES_TYPE::MATERIAL;

	if (typeid(T).hash_code() == typeid(CPrefab).hash_code())
		return RES_TYPE::PREFAB;

	if (typeid(T).hash_code() == typeid(CSound).hash_code())
		return RES_TYPE::SOUND;

	else
		return RES_TYPE::END;
}


template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, T* _pRes)
{
	RES_TYPE eResType = GetType<T>();

	CRes* pRes = FindRes<T>(_strKey).Get();

	assert(!pRes);

	_pRes->SetKey(_strKey);
	m_arrRes[(UINT)eResType].insert(make_pair(_strKey, _pRes));
	m_bChanged = true;
}

template<typename T>
Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
	RES_TYPE eResType = ::GetType<T>();

	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)eResType].find(_strKey);

	if (iter == m_arrRes[(UINT)eResType].end())
	{
		return nullptr;
	}

	return (T*)iter->second.Get();
}

template<typename T>
Ptr<T> CResMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
	RES_TYPE eResType = GetType<T>();

	CRes* pResource = FindRes<T>(_strKey).Get();

	if (nullptr != pResource)
		return (T*)pResource;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	pResource = new T;	
	if (FAILED(pResource->Load(strFilePath)))
	{
		MessageBox(nullptr, strFilePath.c_str(), L"리소스 로딩 실패", MB_OK);
		return nullptr;
	}

	pResource->SetRelativePath(_strRelativePath);
	AddRes<T>(_strKey, (T*)pResource);

	return (T*)pResource;
}

template<typename T>
inline wstring CResMgr::GetNewResName()
{
	RES_TYPE type = GetType<T>();

	wstring strName = L"New ";

	switch (type)
	{
	case RES_TYPE::PREFAB:
		break;
	case RES_TYPE::MESHDATA:
		break;
	case RES_TYPE::COMPUTE_SHADER:
		break;
	case RES_TYPE::MATERIAL:
		strName += L"Material ";
		break;
	case RES_TYPE::MESH:
		break;
	case RES_TYPE::TEXTURE:
		break;
	case RES_TYPE::SOUND:
		break;
	case RES_TYPE::GRAPHICS_SHADER:
		break;
	case RES_TYPE::END:
		break;
	default:
		break;
	}

	
	UINT Count = 1;
	while (true)
	{
		wchar_t szBuff[50] = {};
		_itow_s(Count, szBuff, 10);

		wstring strTemp = strName;
		strTemp += szBuff;

		if (nullptr != FindRes<T>(strTemp))
		{
			++Count;
		}
		else
		{
			strName = strTemp;
			break;
		}		
	}

	return strName;
}

