#pragma once
#include "UI.h"

class TreeUI;

class ContentUI :
    public UI
{
private:
    TreeUI*             m_Tree;
    vector<wstring>     m_vecContentName;


public:
    // 현재 리소스 목록을 트리에 갱신
    void ResetContent();

    // Content 폴더 밑에 파일들을 메모리에 불러옴
    void ReloadContent();


private:
    virtual void update() override;
    virtual void render_update() override;

private:
    void SetResourceToInspector(DWORD_PTR _res);
    void FindContentFileName(const wstring& _strFolderPath);
    RES_TYPE GetResTypeByExt(wstring _filename);

public:
    ContentUI();
    ~ContentUI();
};

