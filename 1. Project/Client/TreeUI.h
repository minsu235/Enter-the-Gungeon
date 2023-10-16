#pragma once
#include "UI.h"

class TreeUI;

class TreeNode
{
private:
    string              m_strName;
    DWORD_PTR           m_data;

    vector<TreeNode*>   m_vecChildNode;
    TreeNode*           m_ParentNode;
    TreeUI*             m_TreeUI;

    UINT                m_iIdx;

    bool                m_bFrame;
    bool                m_bSelected;

public:    
    DWORD_PTR GetData() { return m_data; }
    const string& GetName() { return m_strName; }

private:
    void render_update();
    void SetNodeName(const string& _name) { m_strName = _name; }
    void SetData(DWORD_PTR _data) { m_data = _data; }
    void SetFrame(bool _bFrame) { m_bFrame = _bFrame; }
    void AddChild(TreeNode* _ChildNode)
    {
        _ChildNode->m_ParentNode = this;
        m_vecChildNode.push_back(_ChildNode);
    }

   
    const vector<TreeNode*>& GetChild() { return m_vecChildNode; }


public:
    TreeNode();
    ~TreeNode();
        
    friend class TreeUI;
};



class TreeUI :
    public UI
{
private:
    static UINT m_iNextNodeIdx;

private:
    TreeNode*   m_RootNode;
    bool        m_bDummyRootUse;    // ��Ʈ��带 ���̷� ����Ұ��ΰ�
        
    TreeNode*   m_SelectedNode;
    TreeNode*   m_BeginDragNode;    // �巡�� ���� ���
    TreeNode*   m_DropTargetNode;   // ��� ��� ���

    UI*         m_SelectInst;
    FUNC_1      m_SelectFunc;

    UI*         m_DragDropInst;
    FUNC_2      m_DragDropFunc;
    


public:
    virtual void update() override;
    virtual void render_update() override;

public:
    TreeNode* AddItem(TreeNode* _parent, const string& _strName, DWORD_PTR _data, bool _IsFrame = false);
    void Clear();
    void SetDummyRoot(bool _bUse) { m_bDummyRootUse = _bUse; }
    void SetSelectedNode(TreeNode* _SelectedNode);
    void SetBeginDragNode(TreeNode* _BeginDragNode) { m_BeginDragNode = _BeginDragNode; }
    void SetDropTargetNode(TreeNode* _DropTargetNode);

    TreeNode* GetSelectedNode()  { return m_SelectedNode; }

    void AddDynamic_Selected(UI* _Inst, FUNC_1 _Func)
    {
        m_SelectInst = _Inst;
        m_SelectFunc = _Func;
    }

    void AddDynamic_DragDrop(UI* _Inst, FUNC_2 _Func)
    {
        m_DragDropInst = _Inst;
        m_DragDropFunc = _Func;
    }
    

public:
    TreeUI(const string& _strName);
    ~TreeUI();
};

