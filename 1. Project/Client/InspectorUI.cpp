#include "pch.h"
#include "InspectorUI.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>

#include <Engine\CResMgr.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "ParticleSystemUI.h"
#include "CameraUI.h"
#include "TileMapUI.h"
#include "Animator2DUI.h"

#include "MeshUI.h"
#include "TextureUI.h"
#include "MaterialUI.h"

#include "ScriptUI.h"


InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_TargetObj(nullptr)
	, m_arrComUI{}
	, m_arrResUI{}
{
	// ComponentUI
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;	
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(ImVec2(0.f, 340.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new ParticleSystemUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetSize(ImVec2(0.f, 380.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(ImVec2(0.f, 180.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(ImVec2(0.f, 250.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

	// ResourceUI
	m_arrResUI[(UINT)RES_TYPE::MESH] = new MeshUI;
	m_arrResUI[(UINT)RES_TYPE::MESH]->SetSize(ImVec2(0.f, 0.f));
	m_arrResUI[(UINT)RES_TYPE::MESH]->ShowSeperator(false);
	m_arrResUI[(UINT)RES_TYPE::MESH]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::MESH]);

	m_arrResUI[(UINT)RES_TYPE::TEXTURE] = new TextureUI;
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->SetSize(ImVec2(0.f, 0.f));
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->ShowSeperator(false);
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::TEXTURE]);

	m_arrResUI[(UINT)RES_TYPE::MATERIAL] = new MaterialUI;
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->SetSize(ImVec2(0.f, 0.f));
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->ShowSeperator(false);
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::MATERIAL]);

	ScriptUI* pScriptUI = new ScriptUI;
	pScriptUI->Close();
	AddChild(pScriptUI);
	m_vecScriptUI.push_back(pScriptUI);
}

InspectorUI::~InspectorUI()
{	
}

void InspectorUI::update()
{
	// 타겟 오브젝트가 데드상태인지 확인
	if (!IsValid(m_TargetObj))
	{
		SetTargetObject(nullptr);
	}

	UI::update();
}

void InspectorUI::render_update()
{

}


void InspectorUI::SetTargetObject(CGameObject* _Target)
{
	// 리소스가 타겟인 상태였다면
	if (nullptr != _Target && nullptr != m_TargetRes)
	{
		SetTargetResource(nullptr);
	}

	m_TargetObj = _Target;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			if (nullptr != m_TargetObj && nullptr == m_TargetObj->GetComponent((COMPONENT_TYPE)i))
			{
				m_arrComUI[i]->SetTarget(nullptr);
				m_arrComUI[i]->Close();
			}			
			else
			{
				m_arrComUI[i]->SetTarget(_Target);

				if(nullptr != _Target)
					m_arrComUI[i]->Open();
				else
					m_arrComUI[i]->Close();
			}
		}		
	}	
	

	// Object 의 스크립트 확인
	if (nullptr == m_TargetObj)
	{
		for (auto& pScript : m_vecScriptUI)
		{
			pScript->Close();
		}		
	}

	else
	{
		// 모든 스크립트 UI 비활성화
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->Close();			
		}

		// 오브젝트의 보유 스크립트 가져옴
		const vector<CScript*>& vecScript = m_TargetObj->GetScripts();

		// 스트립트 수가 스트립트UI 보다 많으면, 추가로 UI 를 더 생성
		if (m_vecScriptUI.size() < vecScript.size())
		{
			for (size_t i = 0; i < vecScript.size() - m_vecScriptUI.size(); ++i)
			{
				ScriptUI* pScriptUI = new ScriptUI;
				pScriptUI->Close();
				AddChild(pScriptUI);
				m_vecScriptUI.push_back(pScriptUI);
			}
		}

		// 스크립트를 스크립트UI 에 배정
		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			m_vecScriptUI[i]->Open();
			m_vecScriptUI[i]->SetTargetScript(vecScript[i]);
		}
	}
	
}

void InspectorUI::SetTargetResource(CRes* _Resource)
{
	// Object 가 타겟인 상태였다면
	if (nullptr != m_TargetObj)
	{
		SetTargetObject(nullptr);
	}


	if (nullptr != _Resource)
	{
		// 기존에 가리키던 리소스가 있으면, 해당 UI를 끄고
		if (nullptr != m_TargetRes && nullptr != m_arrResUI[(UINT)m_TargetRes->GetResType()])
		{
			m_arrResUI[(UINT)m_TargetRes->GetResType()]->Close();
		}			

		// 새로 지정된 리소스를 담당하는 UI 를 활성화
		m_TargetRes = _Resource;
		RES_TYPE eType = m_TargetRes->GetResType();

		if (nullptr != m_arrResUI[(UINT)eType])
		{
			m_arrResUI[(UINT)eType]->SetTarget(m_TargetRes);
			m_arrResUI[(UINT)eType]->Open();
		}		
	}
	else
	{
		for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
		{
			if (nullptr != m_arrResUI[i])
			{
				m_arrResUI[i]->SetTarget(nullptr);
				m_arrResUI[i]->Close();
			}			
		}		
	}	
}





