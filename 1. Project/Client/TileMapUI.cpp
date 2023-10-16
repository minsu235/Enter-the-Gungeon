#include "pch.h"
#include "TileMapUI.h"

#include <Engine\CResMgr.h>
#include <Engine\CTileMap.h>
#include <Engine\CGameObject.h>
#include <Engine\CScript.h>
#include <Engine\CEventMgr.h>
#include <Script\CScriptMgr.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "TileUI.h"

TileMapUI::TileMapUI()
	: ComponentUI("TileMap", COMPONENT_TYPE::TILEMAP)
{
}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::update()
{
	if (nullptr != GetTarget())
	{
		m_AtlasTex = GetTarget()->TileMap()->GetTileAtlas();
		m_vSlice = GetTarget()->TileMap()->GetTileSlice();
		m_vTileCount = GetTarget()->TileMap()->GetTileCount();
	}

	ComponentUI::update();
}

void TileMapUI::render_update()
{
	ComponentUI::render_update();

	string TexName;
	if (nullptr != m_AtlasTex)
	{
		TexName = string(m_AtlasTex->GetKey().begin(), m_AtlasTex->GetKey().end());
	}

	ImGui::Text("Texture    ");
	ImGui::SameLine();
	ImGui::InputText("##TexName", (char*)TexName.data(), TexName.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##TexBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메쉬 목록을 받아와서, ListUI 에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&TileMapUI::SetTexture);

		pListUI->Open();
	}
	Vec2 _tilecount = m_vTileCount;
	Vec2 _slicesize = m_vSlice;
	ImGui::Text("TileCount  "); ImGui::SameLine(); ImGui::InputFloat2("x,y##tilecountx", &m_vTileCount.x);
	ImGui::Text("SliceSize  "); ImGui::SameLine(); ImGui::InputFloat2("x,y##DefaultSize", &m_vSlice.x);

	ImGui::Text("TileEdit   ");
	ImGui::SameLine();
	if (ImGui::Button("##Tileeditbtn", Vec2(15.f, 15.f)))
	{
		TileUI* pTileUI = dynamic_cast<TileUI*>(CImGuiMgr::GetInst()->FindUI("TileUI"));
		assert(pTileUI);
		pTileUI->setTarget(GetTarget());
		pTileUI->setTexture(m_AtlasTex);
		pTileUI->setCount(m_vTileCount);
		pTileUI->setSlice(m_vSlice);

		pTileUI->Open();
	}

	if (GetTarget())
	{
		if (_tilecount != m_vTileCount || _slicesize != m_vSlice)
		{
			GetTarget()->TileMap()->SetTileSlice(m_vSlice);
			GetTarget()->TileMap()->SetTileCount((UINT)m_vTileCount.x, (UINT)m_vTileCount.y);
		}
	}

	if (ImGui::Button("CreateWall##createwall", Vec2(100.f, 18.f)))
	{
		CGameObject* pWall = new CGameObject;
		pWall->SetName(L"Wall");
		pWall->AddComponent(new CTransform);
		pWall->AddComponent(new CCollider2D);
		tEvent evt = {};
		evt.eType = EVENT_TYPE::ADD_CHILD;
		evt.wParam = (DWORD_PTR)pWall;
		evt.lParam = (DWORD_PTR)GetTarget();
		CEventMgr::GetInst()->AddEvent(evt);
	}
	ImGui::SameLine();
	if (ImGui::Button("CreateCliff##createCliff", Vec2(100.f, 18.f)))
	{
		CGameObject* pCliff = new CGameObject;
		pCliff->SetName(L"Cliff");
		pCliff->AddComponent(new CTransform);
		pCliff->AddComponent(new CCollider2D);
		tEvent evt = {};
		evt.eType = EVENT_TYPE::ADD_CHILD;
		evt.wParam = (DWORD_PTR)pCliff;
		evt.lParam = (DWORD_PTR)GetTarget();
		CEventMgr::GetInst()->AddEvent(evt);
	}
	if (ImGui::Button("Save##Tilesavebtn", Vec2(45.f, 18.f)))
	{
		SaveTilemap();
	}
}

void TileMapUI::SetTexture(DWORD_PTR _strTexKey)
{
	string strKey = (char*)_strTexKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);
	assert(nullptr != pTex);

	GetTarget()->TileMap()->SetTileAtlas(pTex);
}

void TileMapUI::SaveTilemap()
{
	// 경로
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"map\\Test.map";

	// 파일 쓰기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	SaveGameObject(GetTarget(), pFile);


	fclose(pFile);
}

void TileMapUI::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// 오브젝트 이름 저장
	_Object->SaveToFile(_File);

	// 오브젝트 소유 컴포넌트 저장
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


	// Script 개수, 각 Script 이름 저장
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t iScriptCount = vecScript.size();
	fwrite(&iScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		SaveWStringToFile(CScriptMgr::GetScriptName(vecScript[i]), _File);
		vecScript[i]->SaveToFile(_File);
	}


	// 자식 오브젝트
	const vector<CGameObject*> vecChild = _Object->GetChildObject();

	size_t iChildCount = vecChild.size();
	fwrite(&iChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _File);
	}
}
