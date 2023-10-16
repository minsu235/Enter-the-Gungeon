#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CParticleSystem.h>

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
	, m_fSpawnRange(0)
	, m_Frequency(0.f)
	, m_SpawnDir(0.f)
	, m_SpawnDirRange(0.f)
	, m_WorldSpawn(1)
	, m_vMinMaxSpeed{}
	, m_vMinMaxLifeTime{}
{
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::update()
{
	if (nullptr != GetTarget())
	{
		m_vStartScale = GetTarget()->ParticleSystem()->GetStartScale();
		m_vEndScale = GetTarget()->ParticleSystem()->GetEndScale();
		m_vStartColor = GetTarget()->ParticleSystem()->GetStartColor();
		m_vEndColor = GetTarget()->ParticleSystem()->GetEndColor();

		m_vMinMaxSpeed[0] = GetTarget()->ParticleSystem()->GetMinMaxSpeed().x;
		m_vMinMaxSpeed[1] = GetTarget()->ParticleSystem()->GetMinMaxSpeed().y;
		m_vMinMaxLifeTime[0] = GetTarget()->ParticleSystem()->GetMinMaxLifeTime().x;
		m_vMinMaxLifeTime[1] = GetTarget()->ParticleSystem()->GetMinMaxLifeTime().y;

		m_fSpawnRange = (int)GetTarget()->ParticleSystem()->GetSpawnRange();

		m_Frequency = GetTarget()->ParticleSystem()->GetFrequency();
		m_SpawnDir = GetTarget()->ParticleSystem()->GetSpawnDir();
		m_SpawnDirRange = GetTarget()->ParticleSystem()->GetSpawnDirRange();

		m_WorldSpawn = GetTarget()->ParticleSystem()->GetWorldSpawn();
	}

	ComponentUI::update();
}

void ParticleSystemUI::render_update()
{
	ComponentUI::render_update();
	ImGui::Text("StartScale    "); ImGui::SameLine(); ImGui::InputFloat3("##Startscale", m_vStartScale);
	ImGui::Text("EndScale      "); ImGui::SameLine(); ImGui::InputFloat3("##Endscale", m_vEndScale);
	ImGui::Text("StartColor    ");
	ImGui::ColorEdit3("Color##1", (float*)&m_vStartColor);
	ImGui::Text("EndColor      ");
	ImGui::ColorEdit3("Color##2", (float*)&m_vEndColor);
	ImGui::Text("MinSpeed / MaxSpeed");
	ImGui::InputFloat2("##speed input float2", m_vMinMaxSpeed);
	ImGui::Text("MinLifeTime / MaxLifeTime");
	ImGui::InputFloat2("##life time input float2", m_vMinMaxLifeTime);
	ImGui::Text("SpawnRange    ");
	ImGui::DragInt("(0 -> 10000)##2", &m_fSpawnRange, 0.5f, 0, 10000, "%d");
	ImGui::Text("Frequency     "); ImGui::SameLine(); ImGui::InputFloat("##frequency", &m_Frequency);
	ImGui::Text("SpawnAxisDir  "); ImGui::SliderFloat("degree (0 -> 360)##1", &m_SpawnDir, 0.0f, 360.0f, "%.3f");
	ImGui::Text("SpawnAxisRange"); ImGui::SliderFloat("degree (0 -> 360)##2", &m_SpawnDirRange, 0.0f, 360.0f, "%.3f");

	if (GetTarget())
	{
		GetTarget()->ParticleSystem()->SetStartScale(m_vStartScale);
		GetTarget()->ParticleSystem()->SetEndScale(m_vEndScale);
		GetTarget()->ParticleSystem()->SetStartColor(m_vStartColor);
		GetTarget()->ParticleSystem()->SetEndColor(m_vEndColor);
		GetTarget()->ParticleSystem()->SetMinMaxSpeed(Vec2(m_vMinMaxSpeed[0], m_vMinMaxSpeed[1]));
		GetTarget()->ParticleSystem()->SetMinMaxLifeTime(Vec2(m_vMinMaxLifeTime[0], m_vMinMaxLifeTime[1]));
		GetTarget()->ParticleSystem()->SetSpawnRange((float)m_fSpawnRange);
		GetTarget()->ParticleSystem()->SetFrequency(m_Frequency);
		GetTarget()->ParticleSystem()->SetSpawnDir(m_SpawnDir);
		GetTarget()->ParticleSystem()->SetSpawnDirRange(m_SpawnDirRange);
	}
}