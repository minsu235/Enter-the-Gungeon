#include "pch.h"
#include "Collider2DUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CCollider2D.h>

Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2D", COMPONENT_TYPE::COLLIDER2D)
	, m_bIgnorObjectScale(false)
	, m_coll_item_current(0)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::update()
{
	if (nullptr != GetTarget())
	{
		m_vOffsetPos = GetTarget()->Collider2D()->GetOffsetPos();
		m_vScale = GetTarget()->Collider2D()->GetScale();
		m_vRot = GetTarget()->Collider2D()->GetRotation();
		m_eType = GetTarget()->Collider2D()->GetCollider2DType();
		m_bIgnorObjectScale = GetTarget()->Collider2D()->IsIgnoreObjScale();
		m_coll_item_current = (int)GetTarget()->Collider2D()->GetCollider2DType();
	}

	ComponentUI::update();
}

void Collider2DUI::render_update()
{
	ComponentUI::render_update();
	float vOffsetPos[2] = { m_vOffsetPos.x, m_vOffsetPos.y };
	float vScale[2] = { m_vScale.x , m_vScale.y };
	ImGui::Text("Offset  "); ImGui::SameLine(); ImGui::InputFloat2("##Offset", vOffsetPos);
	ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::InputFloat2("##Scale", vScale);

	m_vRot.ToDegree();
	ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::InputFloat3("##Rotation", m_vRot);

	ImGui::Text("Ignore Object Scale"); ImGui::SameLine(); ImGui::Checkbox("##IgnorObjScale", &m_bIgnorObjectScale);
	const char* items[] = { "Rect", "Circle" };
	ImGui::Text("Type	 "); ImGui::SameLine(); ImGui::Combo("##type", &m_coll_item_current, items, 2);

	if (GetTarget())
	{
		GetTarget()->Collider2D()->SetOffsetPos(Vec2(vOffsetPos[0], vOffsetPos[1]));
		GetTarget()->Collider2D()->SetScale(Vec2(vScale[0], vScale[1]));

		m_vRot.ToRadian();
		GetTarget()->Collider2D()->SetRotation(m_vRot);

		GetTarget()->Collider2D()->SetIgnoreObjectScale(m_bIgnorObjectScale);
		GetTarget()->Collider2D()->SetCollider2DType((COLLIDER2D_TYPE)m_coll_item_current);
	}
}

void Collider2DUI::SetMesh(DWORD_PTR _strMeshKey)
{
}

void Collider2DUI::SetMaterial(DWORD_PTR _strMaterialKey)
{
}