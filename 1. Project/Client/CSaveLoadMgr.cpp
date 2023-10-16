#include "pch.h"
#include "CSaveLoadMgr.h"


#include <Engine\CPathMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\GlobalComponent.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CPrefab.h>

#include <Script\CScriptMgr.h>
#include <Engine\CScript.h>


CSaveLoadMgr::CSaveLoadMgr()
{

}

CSaveLoadMgr::~CSaveLoadMgr()
{

}


void CSaveLoadMgr::init()
{
	CPrefab::Save_GameObject_Func = &CSaveLoadMgr::SaveGameObject;
	CPrefab::Load_GameObject_Func = &CSaveLoadMgr::LoadGameObject;
}

void CSaveLoadMgr::SaveLevel(CLevel* _Level, wstring _strRelativePath)
{
	assert(_Level);

	// ���
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// ���� ����
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	// ���� �̸� ����
	_Level->SaveToFile(pFile);

	// ������ ���̾� ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� �̸� ����
		CLayer* pLayer = _Level->GetLayer(i);
		pLayer->SaveToFile(pFile);

		// ���̾ �ִ� �θ� ������Ʈ ����
		const vector<CGameObject*>& vecParentObj = pLayer->GetParentObjects();

		size_t iObjectCount = vecParentObj.size();
		fwrite(&iObjectCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < vecParentObj.size(); ++j)
		{
			SaveGameObject(vecParentObj[j], pFile);
		}
	}

	// �浹 �Ŵ��� ���� ��
	//CCollisionMgr::GetInst()->SaveToFile(pFile);


	fclose(pFile);
}

void CSaveLoadMgr::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// ������Ʈ �̸� ����
	_Object->SaveToFile(_File);

	// ������Ʈ ���� ������Ʈ ����
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr != pComponent)
		{
			pComponent->SaveToFile(_File);
		}
	}

	COMPONENT_TYPE ComponentEnd = COMPONENT_TYPE::END;
	fwrite(&ComponentEnd, sizeof(UINT), 1, _File);


	// Script ����, �� Script �̸� ����
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t iScriptCount = vecScript.size();
	fwrite(&iScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScript.size(); ++i)
	{		
		SaveWStringToFile(CScriptMgr::GetScriptName(vecScript[i]), _File);
		vecScript[i]->SaveToFile(_File);
	}


	// �ڽ� ������Ʈ
	const vector<CGameObject*> vecChild = _Object->GetChildObject();

	size_t iChildCount = vecChild.size();
	fwrite(&iChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _File);
	}
}




CLevel* CSaveLoadMgr::LoadLevel(wstring _strRelativePath)
{
	// ���
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// ���� �б�
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	// ���� �̸�
	CLevel* pLevel = new CLevel;
	pLevel->LoadFromFile(pFile);

	// ������ ���̾�
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� �̸�
		CLayer* pLayer = pLevel->GetLayer(i);
		pLayer->LoadFromFile(pFile);

		// ���̾ �ִ� �θ� ������Ʈ 
		size_t iObjectCount = 0;		
		fread(&iObjectCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iObjectCount; ++j)
		{			
			CGameObject* pObject = LoadGameObject(pFile);
			pLayer->AddGameObject(pObject);
		}
	}

	// �浹 �Ŵ��� ���� ��
	//CCollisionMgr::GetInst()->SaveToFile(pFile);


	fclose(pFile);



	return pLevel;
}



CGameObject* CSaveLoadMgr::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// ������Ʈ �̸�
	pObject->LoadFromFile(_File);

	// ������Ʈ ���� ������Ʈ
	bool bProgress = true;
	while (true)
	{
		// Component Ÿ�� Ȯ��
		COMPONENT_TYPE type = COMPONENT_TYPE::END;
		fread(&type, sizeof(UINT), 1, _File);

		// Ÿ�Կ� �ش��ϴ� ������Ʈ �ε� �� �߰�
		CComponent* pComponent = nullptr;

		switch (type)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new CTransform;			
			break;

		case COMPONENT_TYPE::CAMERA:
			pComponent = new CCamera;
			break;

		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new CCollider2D;
			break;
	
		case COMPONENT_TYPE::ANIMATOR2D:
			pComponent = new CAnimator2D;
			break;
		
		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new CLight2D;
			break;
		
		case COMPONENT_TYPE::RIGIDBODY:
			pComponent = new CRigidBody;
			break;

		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new CMeshRender;
			break;

		case COMPONENT_TYPE::TILEMAP:
			pComponent = new CTileMap;
			break;

		case COMPONENT_TYPE::PARTICLESYSTEM:
			pComponent = new CParticleSystem;
			break;

		case COMPONENT_TYPE::LIGHT3D:
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			break;
		case COMPONENT_TYPE::SKYBOX:
			break;
		case COMPONENT_TYPE::DECAL:
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			break;
		case COMPONENT_TYPE::END:
			bProgress = false;
			break;		
		}

		if (!bProgress)
			break;

		pComponent->LoadFromFile(_File);
		pObject->AddComponent(pComponent);
	}


	// Script
	size_t iScriptCount = 0;
	fread(&iScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iScriptCount; ++i)
	{
		wstring strScriptName;
		LoadWStringFromFile(strScriptName, _File);
		CScript* pNewScript = CScriptMgr::GetScript(strScriptName);
		pNewScript->LoadFromFile(_File);

		pObject->AddComponent(pNewScript);
	}


	// �ڽ� ������Ʈ
	size_t iChildCount = 0;
	fread(&iChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iChildCount; ++i)
	{
		CGameObject* pChild = LoadGameObject(_File);
		pObject->AddChild(pChild);
	}

	return pObject;
}
