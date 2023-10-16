#include "pch.h"
#include "Light2DUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CLight2D.h>

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", COMPONENT_TYPE::LIGHT2D)
	, m_vColor{}
	, m_Radius(0)
{
}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::update()
{
	if (nullptr != GetTarget())
	{
		m_vColor[0] = GetTarget()->Light2D()->GetLightColor().x;
		m_vColor[1] = GetTarget()->Light2D()->GetLightColor().y;
		m_vColor[2] = GetTarget()->Light2D()->GetLightColor().z;
		m_vColor[3] = 1.f;
		m_Radius = (int)GetTarget()->Light2D()->GetRadius();
	}

	ComponentUI::update();
}

void Light2DUI::render_update()
{
	ComponentUI::render_update();
	ImGui::Text("Radius  ");
	ImGui::DragInt("(0 -> 10000)", &m_Radius, 0.5f, 0, 10000, "%d");
	ImGui::Text("Color   ");
	ImGuiColorEditFlags flags = 0;
	flags |= ImGuiColorEditFlags_NoAlpha;
	flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
	ImGui::ColorPicker4("Color##Light2D", m_vColor, flags);

	if (GetTarget())
	{
		GetTarget()->Light2D()->SetLightColor(Vec3(m_vColor[0], m_vColor[1], m_vColor[2]));
		GetTarget()->Light2D()->SetRadius((float)m_Radius);
	}
}
