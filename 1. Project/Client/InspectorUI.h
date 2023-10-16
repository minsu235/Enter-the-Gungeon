#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;
class ScriptUI;

#include <Engine\Ptr.h>
#include <Engine\CRes.h>
class ResUI;


class InspectorUI :
    public UI
{
private:
    CGameObject*        m_TargetObj;   
    ComponentUI*        m_arrComUI[(UINT)COMPONENT_TYPE::END];

    vector<ScriptUI*>   m_vecScriptUI;


    Ptr<CRes>           m_TargetRes;
    ResUI*              m_arrResUI[(UINT)RES_TYPE::END]; 
    

public:
    CGameObject* GetTargetObject() { return m_TargetObj; }


    void SetTargetObject(CGameObject* _Target);
    void SetTargetResource(CRes* _Resource);


private:
    virtual void update() override;
    virtual void render_update() override;

public:
    InspectorUI();
    ~InspectorUI();
};

