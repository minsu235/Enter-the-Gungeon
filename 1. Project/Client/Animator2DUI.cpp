#include "pch.h"
#include "Animator2DUI.h"
#include <Engine\CGameObject.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CAnimation2D.h>
#include <Engine\CTimeMgr.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "AnimUI.h"

Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2D", COMPONENT_TYPE::ANIMATOR2D)
	, m_AccTime(0.f)
	, m_Animation(nullptr)
	, m_iCurIdx(0)
{
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::update()
{
	if (nullptr != GetTarget())
	{
		if (nullptr == m_Animation)
		{
			if (!GetTarget()->Animator2D()->GetMapAnimation().empty())
			{
				m_Animation = GetTarget()->Animator2D()->GetMapAnimation().begin()->second;
			}
		}

		if (nullptr != m_Animation)
		{
			m_AtlasTex = m_Animation->GetAnimAtlas();
		}
	}
	ComponentUI::update();
}

void Animator2DUI::render_update()
{
	ComponentUI::render_update();

	string AnimName;
	if (nullptr != m_Animation)
	{
		AnimName = string(m_Animation->GetName().begin(), m_Animation->GetName().end());
	}

	ImGui::Text("Animation Name");
	ImGui::SameLine();
	ImGui::InputText("##AnimName", (char*)AnimName.data(), AnimName.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##AnimBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메쉬 목록을 받아와서, ListUI 에 전달
		const map<wstring, CAnimation2D*>& mapAni = GetTarget()->Animator2D()->GetMapAnimation();
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, CAnimation2D*>::const_iterator iter = mapAni.begin();
		for (; iter != mapAni.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&Animator2DUI::SetAnimaion);

		pListUI->Open();
	}

	m_AccTime += DT;

	ImGui::Text("Animation Play");
	vector<tAnim2DFrm> vecAnim = m_Animation->GetFrmVec();

	if (vecAnim[m_iCurIdx].fDuration < m_AccTime)
	{
		m_AccTime = 0.f;
		++m_iCurIdx;

		// 최대 프레임에 도달하면 Finish 상태로 전환
		if (vecAnim.size() <= m_iCurIdx)
		{
			m_iCurIdx = 0;
		}
	}
	ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2(150.f, 150.f),
		ImVec2(vecAnim[m_iCurIdx].vLeftTop.x, vecAnim[m_iCurIdx].vLeftTop.y),
		ImVec2(vecAnim[m_iCurIdx].vLeftTop.x + vecAnim[m_iCurIdx].vSlice.x, vecAnim[m_iCurIdx].vLeftTop.y + vecAnim[m_iCurIdx].vSlice.y));

	if (ImGui::Button("Edit##Animeditbtn", Vec2(35.f, 20.f)))
	{
		AnimUI* pAnimUI = dynamic_cast<AnimUI*>(CImGuiMgr::GetInst()->FindUI("AnimUI"));
		assert(pAnimUI);
		pAnimUI->setTarget(m_Animation);
		pAnimUI->setTexture(m_AtlasTex);
		pAnimUI->setVector(vecAnim);
		pAnimUI->setOwner(this);

		pAnimUI->Open();
	}

	if (ImGui::Button("Save##Tilesavebtn", Vec2(45.f, 18.f)))
	{
		SaveAnimation();
	}
}

void Animator2DUI::SetAnimaion(DWORD_PTR _strAnimKey)
{
	string strKey = (char*)_strAnimKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	m_Animation = GetTarget()->Animator2D()->FindAnimation(wstrKey);
	m_AccTime = 0.f;
	m_iCurIdx = 0;
}

void Animator2DUI::SaveAnimation()
{
	// 경로
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"animation\\Test.anim";

	// 파일 쓰기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	GetTarget()->Animator2D()->SaveToFile(pFile);

	fclose(pFile);
}


