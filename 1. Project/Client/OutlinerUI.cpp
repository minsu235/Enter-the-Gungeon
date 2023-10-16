#include "pch.h"
#include "OutlinerUI.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\CEventMgr.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"


OutlinerUI::OutlinerUI()
	: UI("Outliner")
	, m_Tree(nullptr)
{
	m_Tree = new TreeUI("##OutlinerTree");
	AddChild(m_Tree);

	// 트리 설정
	m_Tree->ShowSeperator(false);
	m_Tree->SetDummyRoot(true);

	// Tree 에 Delegate 등록
	m_Tree->AddDynamic_Selected(this, (FUNC_1)&OutlinerUI::SetObjectToInspector);
	m_Tree->AddDynamic_DragDrop(this, (FUNC_2)&OutlinerUI::AddChildObject);

	// 레벨 오브젝트 갱신
	ResetLevel();
}

OutlinerUI::~OutlinerUI()
{
}

void OutlinerUI::update()
{
	if (CEventMgr::GetInst()->IsLevelChanged())
	{
		ResetLevel();
	}	
}

void OutlinerUI::render_update()
{
}


void OutlinerUI::ResetLevel()
{
	m_Tree->Clear();

	// 현재 레벨을 가져온다.
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	string strLevelName = string(pCurLevel->GetName().begin(), pCurLevel->GetName().end());

	// 레벨이름으로 루트노드 추가
	TreeNode* pRootNode = m_Tree->AddItem(nullptr, strLevelName.c_str(), 0, true);

	// 모든 레이어를 확인하면서, 부모 오브젝트들 기준으로 트리에 추가
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = pCurLevel->GetLayer(i)->GetParentObjects();

		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			AddGameObjectToTree(pRootNode, vecObject[j]);
		}
	}
}

void OutlinerUI::SetObjectToInspector(DWORD_PTR _res)
{
	// _res : 클릭한 노드
	TreeNode* pSelectedNode = (TreeNode*)_res;
	CGameObject* pObject = (CGameObject*)pSelectedNode->GetData();

	// InspectorUI 에 클릭된 Resouce 를 알려준다.
	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");

	Inspector->SetTargetObject(pObject);
}

void OutlinerUI::AddGameObjectToTree(TreeNode* _ParentNode, CGameObject* _Object)
{
	string strObjectName = string(_Object->GetName().begin(), _Object->GetName().end());
	TreeNode* pCurNode = m_Tree->AddItem(_ParentNode, strObjectName.c_str(), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChildObject();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(pCurNode, vecChild[i]);
	}
}

void OutlinerUI::AddChildObject(DWORD_PTR _ChildObject, DWORD_PTR _ParentObject)
{
	TreeNode* pChildNode = (TreeNode*)_ChildObject;
	TreeNode* pParentNode = (TreeNode*)_ParentObject;

	tEvent evt = {};
	evt.eType = EVENT_TYPE::ADD_CHILD;
	evt.wParam = pChildNode->GetData();
	evt.lParam = pParentNode->GetData();
	CEventMgr::GetInst()->AddEvent(evt);	
}
