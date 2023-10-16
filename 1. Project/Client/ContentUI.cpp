#include "pch.h"
#include "ContentUI.h"

#include <Engine\CResMgr.h>
#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CPathMgr.h>
#include <Engine\CEventMgr.h>


ContentUI::ContentUI()
	: UI("Content")
	, m_Tree(nullptr)
{
	m_Tree = new TreeUI("##ContentTree");
	AddChild(m_Tree);


	// 트리 설정
	m_Tree->ShowSeperator(false);
	m_Tree->SetDummyRoot(true);


	// Tree 에 Delegate 등록
	m_Tree->AddDynamic_Selected(this, (FUNC_1)& ContentUI::SetResourceToInspector);
}

ContentUI::~ContentUI()
{
}


void ContentUI::update()
{
	if (CResMgr::GetInst()->IsChanged())
	{
		ResetContent();
	}

	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::S))
	{
		TreeNode* pSelectedNode = m_Tree->GetSelectedNode();
		if (nullptr != pSelectedNode)
		{			
			RES_TYPE type = ((CRes*)pSelectedNode->GetData())->GetResType();
			
			wstring strRelativePath;

			if (RES_TYPE::MATERIAL == type)
			{
				Ptr<CMaterial> pRes = (CMaterial*)pSelectedNode->GetData();
				
				strRelativePath = L"material//";
				strRelativePath += pRes->GetKey();
				pRes->Save(strRelativePath);
			}
		}
	}

	UI::update();
}

void ContentUI::render_update()
{

}



void ContentUI::ResetContent()
{
	m_Tree->Clear();

	TreeNode* pRootNode = m_Tree->AddItem(nullptr, "Content", 0);
		
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		TreeNode* pResNode = m_Tree->AddItem(pRootNode, ToString((RES_TYPE)i), 0, true);

		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource((RES_TYPE)i);

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			wstring strName = iter->first;
			m_Tree->AddItem(pResNode, string(strName.begin(), strName.end()), (DWORD_PTR)iter->second.Get());
		}
	}
}

void ContentUI::ReloadContent()
{
	m_vecContentName.clear();

	// Content 폴더에 있는 모든 리소스들을 검사 및 로딩
	wstring strFolderPath = CPathMgr::GetInst()->GetContentPath();
	FindContentFileName(strFolderPath);	

	// 파일명의 확장자를 확인 후 알맞은 리소스가 무엇인지 알아낸다.
	for (size_t i = 0; i < m_vecContentName.size(); ++i)
	{
		RES_TYPE resType = GetResTypeByExt(m_vecContentName[i]);

		// 리소스 타입에 해당하는 리소스를 경로로부터 로딩
		if (RES_TYPE::END == resType)
			continue;

		switch (resType)
		{
		case RES_TYPE::PREFAB:
			CResMgr::GetInst()->Load<CPrefab>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::MESHDATA:

			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::MESH:
			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::SOUND:
			CResMgr::GetInst()->Load<CSound>(m_vecContentName[i], m_vecContentName[i]);
			break;
		}
	}

	// 로딩된 리소스가 실제 파일로 존재하는지 확인
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource((RES_TYPE)i);

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (iter; iter != mapRes.end(); ++iter)
		{
			// Engine Res 타입이면 확인하지 않는다.
			if (iter->second->IsEngineRes())
			{
				continue;
			}

			wstring strRelativePath = iter->second->GetRelativePath();
			assert(!strRelativePath.empty());

			// 메모리에 로딩된 리소스에 해당하는 실제 파일이 존재하지 않는 경우
			if (!filesystem::exists(strFolderPath + strRelativePath))
			{
				// 리소스 매니저에서 해당 리소스를 해제한다.
				if (iter->second->GetRefCount() <= 1)
				{
					// 삭제 가능
					tEvent evn = {};
					evn.eType = EVENT_TYPE::DELETE_RES;
					evn.wParam = i;
					evn.lParam = (DWORD_PTR)(iter->second.Get());

					CEventMgr::GetInst()->AddEvent(evn);

					MessageBox(nullptr, L"원본 리소스 삭제됨", L"리소스 변경 확인", MB_OK);
				}
				else 
				{
					// 해당 리소스를 참조하는 객체가 있음	
					MessageBox(nullptr, L"사용 중 인 리소스/n리소스 삭제 실패", L"리소스 변경 확인", MB_OK);
				}
			}
		}
	}	
}


void ContentUI::SetResourceToInspector(DWORD_PTR _res)
{
	// _res : 클릭한 노드
	TreeNode* pSelectedNode = (TreeNode*)_res;
	CRes* pRes = (CRes*)pSelectedNode->GetData();

	// InspectorUI 에 클릭된 Resouce 를 알려준다.
	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");

	Inspector->SetTargetResource(pRes);
}

void ContentUI::FindContentFileName(const wstring& _strFolderPath)
{
	// 모든 파일명을 알아낸다.
	wstring strFolderPath = _strFolderPath + L"*.*";	

	HANDLE hFindHandle = nullptr;

	WIN32_FIND_DATA data = {};	

	// 탐색용 핸들 얻기
	hFindHandle = FindFirstFile(strFolderPath.c_str(), &data);

	if (INVALID_HANDLE_VALUE == hFindHandle)
		return;

	// 탐색 핸들을 이용해서 모든 파일을 다 확인할 때 까지 반복
	while (FindNextFile(hFindHandle, &data))
	{
		if ((FILE_ATTRIBUTE_DIRECTORY & data.dwFileAttributes) && wcscmp(data.cFileName, L".."))
		{
			FindContentFileName(_strFolderPath + data.cFileName + L"\\");
		}

		else
		{
			wstring strRelative = GetRelativePath(CPathMgr::GetInst()->GetContentPath(), _strFolderPath + data.cFileName);
			m_vecContentName.push_back(strRelative);
		}		
	}

	FindClose(hFindHandle);	
}

RES_TYPE ContentUI::GetResTypeByExt(wstring _filename)
{	
	wstring strExt = path(_filename.c_str()).extension();

	if (strExt == L".pref")
		return RES_TYPE::PREFAB;
	else if (strExt == L".mdat")
		return RES_TYPE::MESHDATA;
	else if (strExt == L".mtrl")
		return RES_TYPE::MATERIAL;
	else if (strExt == L".mesh")
		return RES_TYPE::MESH;
	else if (strExt == L".dds" || strExt == L".DDS" || strExt == L".tag" || strExt == L".TGA"
		|| strExt == L".png" || strExt == L".jpg" || strExt == L".JPG" || strExt == L".jegp" || strExt == L".JPEG"
		|| strExt == L".bmp" || strExt == L".BMP")
		return RES_TYPE::TEXTURE;
	else if (strExt == L".mp3" || strExt == L".MP3" || strExt == L".wav" || strExt == L".WAV" || strExt == L".ogg" || strExt == L".OGG")
		return RES_TYPE::SOUND;
	else
		return RES_TYPE::END;
}
