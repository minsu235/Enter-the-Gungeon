#include "pch.h"
#include "CameraUI.h"
#include <Engine\CCamera.h>
#include <Engine\CTransform.h>

CameraUI::CameraUI()
    : ComponentUI("Camera", COMPONENT_TYPE::CAMERA)
    , m_fScale(0.f)
    , m_fFar(0.f)
    , m_proj_item_current(0)
{
}

CameraUI::~CameraUI()
{
}

void CameraUI::update()
{
    if (nullptr != GetTarget())
    {
        m_vPos = GetTarget()->Transform()->GetRelativePos();
        m_vRot = GetTarget()->Transform()->GetRelativeRotation();
        m_fScale = GetTarget()->Camera()->GetOrthographicScale();
        m_fFar = GetTarget()->Camera()->GetFar();
        m_proj_item_current = (int)GetTarget()->Camera()->GetProjType();
    }
    UI::update();
}

void CameraUI::render_update()
{
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
    ImGui::Button("MainCamera");
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::Text("Position        "); ImGui::SameLine(); ImGui::InputFloat3("##Position", m_vPos);
    m_vRot.ToDegree();
    ImGui::Text("Rotation        "); ImGui::SameLine(); ImGui::InputFloat3("##Rotation", m_vRot);
    ImGui::Text("Viewing Distance"); ImGui::SameLine(); ImGui::InputFloat("##Far", &m_fFar);
    const char* items[] = { "PERSPECTIVE", "ORTHOGRAHPICS" };
    ImGui::Text("ProjType        "); ImGui::SameLine(); ImGui::Combo("##proj", &m_proj_item_current, items, 2);

    if (m_proj_item_current == 1)
    {
        ImGui::Text("OrthographicScale"); ImGui::SameLine(); ImGui::InputFloat("##orthoscale", &m_fScale);
    }

    if (GetTarget())
    {
        GetTarget()->Transform()->SetRelativePos(m_vPos);
        if (m_fFar > 1)
            GetTarget()->Camera()->SetFar(m_fFar);
        if (m_proj_item_current == 1)
        {
            m_vRot = Vec3(0.f, 0.f, 0.f);
            if (m_fScale > 0)
                GetTarget()->Camera()->SetOrthographicScale(m_fScale);
        }
        m_vRot.ToRadian();
        GetTarget()->Transform()->SetRelativeRotation(m_vRot);
        GetTarget()->Camera()->SetProjType((PROJ_TYPE)m_proj_item_current);
    }
}