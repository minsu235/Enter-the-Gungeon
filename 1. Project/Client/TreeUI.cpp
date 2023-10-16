#include "pch.h"
#include "TreeUI.h"


// ========
// TreeNode
// ========
TreeNode::TreeNode()
	: m_ParentNode(nullptr)
	, m_data(0)
	, m_TreeUI(nullptr)
	, m_bFrame(false)
	, m_bSelected(false)
	, m_iIdx(0)
{
}

TreeNode::~TreeNode()
{
	Safe_Del_Vec(m_vecChildNode);
}


void TreeNode::render_update()
{
	//ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;

	int iFlag = 0;
	if (m_bFrame)
		iFlag |= ImGuiTreeNodeFlags_Framed;
	if (m_bSelected)
		iFlag |= ImGuiTreeNodeFlags_Selected;
	if (m_vecChildNode.empty())
		iFlag |= ImGuiTreeNodeFlags_Leaf;
	
	string strName = m_strName;
	if (m_bFrame && m_vecChildNode.empty())	
		strName = "\t" + strName;
	
	// 뒤에 식별번호 붙여서 이름(키) 이 같지 않도록 함
	char szTag[50] = "";
	sprintf_s(szTag, 50, "##%d", m_iIdx);
	strName += szTag;

	if (ImGui::TreeNodeEx(strName.c_str(), iFlag))
	{
		// 드래그 체크
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			m_TreeUI->SetBeginDragNode(this);

			ImGui::SetDragDropPayload(m_TreeUI->GetName().c_str(), (void*)this, sizeof(TreeNode));
			ImGui::Text(strName.c_str());

			ImGui::EndDragDropSource();
		}
		else if (!m_bFrame && ImGui::IsItemHovered(0) && ImGui::IsMouseReleased(0))
		{
			m_TreeUI->SetSelectedNode(this);
		}

		// 드랍 체크
		if (ImGui::BeginDragDropTarget())
		{
			m_TreeUI->SetDropTargetNode(this);		

			ImGui::EndDragDropTarget();
		}



		


		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}	
}







// ======
// TreeUI
// ======
UINT TreeUI::m_iNextNodeIdx = 0;

TreeUI::TreeUI(const string& _strName)
	: UI(_strName)
	, m_RootNode(nullptr)
	, m_bDummyRootUse(false)
	, m_SelectedNode(nullptr)
	, m_BeginDragNode(nullptr)
	, m_DropTargetNode(nullptr)
	, m_SelectInst(nullptr)
	, m_SelectFunc(nullptr)	
	, m_DragDropInst(nullptr)
	, m_DragDropFunc(nullptr)
{
}

TreeUI::~TreeUI()
{
	Clear();
}

void TreeUI::update()
{
	
}

void TreeUI::render_update()
{
	if (nullptr == m_RootNode)
		return;

	if (!m_bDummyRootUse)
	{
		m_RootNode->render_update();
	}
	else
	{
		const vector<TreeNode*>& vecChildNode = m_RootNode->GetChild();
		for (size_t i = 0; i < vecChildNode.size(); ++i)
		{
			vecChildNode[i]->render_update();
		}
	}


	// 마우스 왼쪽 릴리즈 체크
	if (ImGui::IsMouseReleased(0))
	{
		m_BeginDragNode = nullptr;
		m_DropTargetNode = nullptr;
	}
}

TreeNode* TreeUI::AddItem(TreeNode* _parent, const string& _strName, DWORD_PTR _data, bool _IsFrame)
{
	TreeNode* pNode = new TreeNode;
	pNode->SetNodeName(_strName);
	pNode->SetData(_data);
	pNode->SetFrame(_IsFrame);
	pNode->m_TreeUI = this;
	pNode->m_iIdx = m_iNextNodeIdx++;

	// 루트로 지정되는 노드
	if (nullptr == _parent)
	{
		assert(!m_RootNode);
		m_RootNode = pNode;
	}
	else
	{
		_parent->AddChild(pNode);
	}

	return pNode;
}

void TreeUI::Clear()
{
	if (nullptr != m_RootNode)
	{
		delete m_RootNode;
		m_RootNode = nullptr;
	}	
}

void TreeUI::SetSelectedNode(TreeNode* _SelectedNode)
{
	if (nullptr != m_SelectedNode)
	{
		m_SelectedNode->m_bSelected = false;
	}

	m_SelectedNode = _SelectedNode;
	m_SelectedNode->m_bSelected = true;

	if (m_SelectInst && m_SelectFunc)
	{
		(m_SelectInst->*m_SelectFunc)( (DWORD_PTR)m_SelectedNode);
	}
}

void TreeUI::SetDropTargetNode(TreeNode* _DropTargetNode)
{
	m_DropTargetNode = _DropTargetNode;

	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetName().c_str()))
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_BeginDragNode, (DWORD_PTR)m_DropTargetNode);
		}
	}

	// 외부 드랍 관련 PayLoad 처리하기

}
