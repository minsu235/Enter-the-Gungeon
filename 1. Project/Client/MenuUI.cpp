#include "pch.h"
#include "MenuUI.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CEventMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CScript.h>
#include <Script\CScriptMgr.h>

#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "ContentUI.h"
#include "CSaveLoadMgr.h"


MenuUI::MenuUI()
	: UI("##MenuUI")
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {           
            if (ImGui::MenuItem("Level Save"))
            {
                // Level ÀúÀå
                CSaveLoadMgr::GetInst()->SaveLevel(CLevelMgr::GetInst()->GetCurLevel(), L"level\\Test.lv");
            }


            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z"))
            {

            }
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Component"))
        {
            if (ImGui::BeginMenu("AddScript"))
            {
                vector<wstring> vecScriptName;
                CScriptMgr::GetScriptInfo(vecScriptName);

                for (size_t i = 0; i < vecScriptName.size(); ++i)
                {
                    string strScriptName = WStringToString(vecScriptName[i]);
                    if (ImGui::MenuItem(strScriptName.c_str()))
                    {
                        InspectorUI* pInspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
                        CGameObject* pTargetObject = pInspector->GetTargetObject();
                        if (nullptr != pTargetObject)
                        {                            
                            pTargetObject->AddComponent(CScriptMgr::GetScript(vecScriptName[i]));
                            pInspector->SetTargetObject(pTargetObject);
                        }                        
                    }
                }

                ImGui::EndMenu();
            }



            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Level"))
        {
            LEVEL_STATE State = CLevelMgr::GetInst()->GetLevelState();

            bool PlayEnable = true;
            bool PauseEnable = true;
            bool StopEnable = true;

            if (LEVEL_STATE::PLAY != State) PlayEnable = true; else PlayEnable = false;
            if (LEVEL_STATE::PLAY == State) PauseEnable = true; else  PauseEnable = false;
            if (LEVEL_STATE::STOP != State) StopEnable = true; else StopEnable = false;

            if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
            {                
                tEvent evn = {};
                evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
                evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
                CEventMgr::GetInst()->AddEvent(evn);               
            }

            if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
            {
                tEvent evn = {};
                evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
                evn.wParam = (DWORD_PTR)LEVEL_STATE::PAUSE;
                CEventMgr::GetInst()->AddEvent(evn);               
            }

            if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
            {
                tEvent evn = {};
                evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
                evn.wParam = (DWORD_PTR)LEVEL_STATE::STOP;
                CEventMgr::GetInst()->AddEvent(evn);                
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Resource"))
        {
            if (ImGui::MenuItem("Create Material"))
            {
                Ptr<CMaterial> pMtrl = new CMaterial;
                wstring strKey = CResMgr::GetInst()->GetNewResName<CMaterial>();
                CResMgr::GetInst()->AddRes<CMaterial>(strKey, pMtrl.Get());

                ContentUI* pContentUI = (ContentUI * )CImGuiMgr::GetInst()->FindUI("Content");
                pContentUI->ResetContent();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }    
}

void MenuUI::render_update()
{
}

