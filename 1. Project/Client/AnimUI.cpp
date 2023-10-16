#include "pch.h"
#include "AnimUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CAnimation2D.h>
#include <Engine\CTimeMgr.h>

#include "CImGuiMgr.h"
#include "Animator2DUI.h"
#include "AnimCursorUI.h"

AnimUI::AnimUI()
	: UI("AnimUI")
	, m_AccTime(0.f)
	, m_iCurIdx(0)
	, m_focusIdx(0)
	, m_bPlay(false)
	, AnimUIFps(0)
	, Animfullsize{}
	, Animlefttop{}
	, Animoffset{}
	, AnimSlice{}
	, m_bStepAnimation(false)
	, AnimStep(0.f)
{
	Close();
	SetSize(ImVec2(300.f, 500.f));

}

AnimUI::~AnimUI()
{
	m_copyvector.clear();
}

void AnimUI::update()
{
	UI::update();


}

void AnimUI::render_update()
{
	if (m_copyvector.empty())
	{
		if (ImGui::Button("Add##AnimAdd", Vec2(35.f, 20.f)))
		{
			tAnim2DFrm frm = {};
			m_copyvector.push_back(frm);
		}
		return;
	}

	float fWidth = (float)m_AtlasTex->GetWidth();
	float fHeight = (float)m_AtlasTex->GetHeight();

	ImGui::Checkbox("Step Animation", &m_bStepAnimation);
	if (m_bStepAnimation == false)
	{
		ImGui::SliderInt("Idx##focusAnimIdx", &m_iCurIdx, 0, (int)m_copyvector.size() - 1);
		if (m_bPlay)
		{
			m_AccTime += DT;

			if (m_copyvector[m_iCurIdx].fDuration < m_AccTime)
			{
				m_AccTime = 0.f;
				++m_iCurIdx;

				if (m_copyvector.size() <= m_iCurIdx)
				{
					m_iCurIdx = 0;
				}
			}
		}
		ImGui::Begin("AnimationPlay");
		{
			ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2(m_copyvector[m_iCurIdx].vSlice.x * fWidth, m_copyvector[m_iCurIdx].vSlice.y * fHeight),
				ImVec2(m_copyvector[m_iCurIdx].vLeftTop.x + m_copyvector[m_iCurIdx].vOffset.x, m_copyvector[m_iCurIdx].vLeftTop.y + m_copyvector[m_iCurIdx].vOffset.y),
				ImVec2(m_copyvector[m_iCurIdx].vLeftTop.x + m_copyvector[m_iCurIdx].vOffset.x + m_copyvector[m_iCurIdx].vSlice.x, m_copyvector[m_iCurIdx].vLeftTop.y + m_copyvector[m_iCurIdx].vOffset.y + m_copyvector[m_iCurIdx].vSlice.y));
			ImGui::End();
		}


		if (ImGui::Button("Play##AnimPlay", Vec2(35.f, 20.f)))
		{
			m_bPlay = true;
			m_focusIdx = m_iCurIdx;
			m_iCurIdx = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop##AnimStop", Vec2(35.f, 20.f)))
		{
			m_bPlay = false;
			m_AccTime = 0;
			m_iCurIdx = m_focusIdx;
		}

		//ImGui::PushItemWidth(100.f);

		ImGui::Text("Fps     "); ImGui::SameLine();
		AnimUIFps = (int)(1 / m_copyvector[m_iCurIdx].fDuration);
		ImGui::InputInt("##frameduration", &AnimUIFps);

		Animfullsize[0] = m_copyvector[m_iCurIdx].vFullSize.x * fWidth;
		Animfullsize[1] = m_copyvector[m_iCurIdx].vFullSize.y * fHeight;

		ImGui::Text("FullSize"); ImGui::SameLine(); ImGui::InputFloat2("##AnimFullSize", Animfullsize);

		Animlefttop[0] = m_copyvector[m_iCurIdx].vLeftTop.x * fWidth;
		Animlefttop[1] = m_copyvector[m_iCurIdx].vLeftTop.y * fHeight;
		ImGui::Text("LeftTop "); ImGui::SameLine(); ImGui::InputFloat2("##AnimLefttop", Animlefttop);
		if (!m_bPlay)
		{
			ImGui::SameLine();
			if (ImGui::Button("##texturecursor", Vec2(15.f, 15.f)))
			{
				AnimCursorUI* pAnimCursorUI = dynamic_cast<AnimCursorUI*>(CImGuiMgr::GetInst()->FindUI("AnimCursorUI"));
				assert(pAnimCursorUI);
				pAnimCursorUI->setTexture(m_AtlasTex);
				pAnimCursorUI->setOwner(this);
				pAnimCursorUI->setAnimSlice(AnimSlice);
				pAnimCursorUI->Open();
			}
		}

		Animoffset[0] = m_copyvector[m_iCurIdx].vOffset.x * fWidth;
		Animoffset[1] = m_copyvector[m_iCurIdx].vOffset.y * fHeight;
		ImGui::Text("Offset  "); ImGui::SameLine(); ImGui::InputFloat2("##Animoffset", Animoffset);

		AnimSlice[0] = m_copyvector[m_iCurIdx].vSlice.x * fWidth;
		AnimSlice[1] = m_copyvector[m_iCurIdx].vSlice.y * fHeight;
		ImGui::Text("Slice   "); ImGui::SameLine(); ImGui::InputFloat2("##Animslice", AnimSlice);

		if (!m_bPlay)
		{
			m_copyvector[m_iCurIdx].vLeftTop = Vec2(Animlefttop[0] / fWidth, Animlefttop[1] / fHeight);
			m_copyvector[m_iCurIdx].vOffset = Vec2(Animoffset[0] / fWidth, Animoffset[1] / fHeight);
			m_copyvector[m_iCurIdx].vSlice = Vec2(AnimSlice[0] / fWidth, AnimSlice[1] / fHeight);
		}
		if (m_copyvector[m_iCurIdx].vSlice.x > m_copyvector[m_iCurIdx].vFullSize.x)
		{
			Animfullsize[0] = m_copyvector[m_iCurIdx].vSlice.x * fWidth;
		}

		if (m_copyvector[m_iCurIdx].vSlice.y > m_copyvector[m_iCurIdx].vFullSize.y)
		{
			Animfullsize[1] = m_copyvector[m_iCurIdx].vSlice.y * fHeight;
		}
	}
	else
	{
		if (m_bPlay)
		{
			m_AccTime += DT;

			if (m_copyvector[m_iCurIdx].fDuration < m_AccTime)
			{
				m_AccTime = 0.f;
				++m_iCurIdx;

				if (m_copyvector.size() <= m_iCurIdx)
				{
					m_iCurIdx = 0;
				}
			}
		}

		ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2(150.f, 150.f),
			ImVec2(m_copyvector[m_iCurIdx].vLeftTop.x, m_copyvector[m_iCurIdx].vLeftTop.y),
			ImVec2(m_copyvector[m_iCurIdx].vLeftTop.x + m_copyvector[m_iCurIdx].vSlice.x, m_copyvector[m_iCurIdx].vLeftTop.y + m_copyvector[m_iCurIdx].vSlice.y));
		ImGui::NewLine();
		if (ImGui::Button("Play##AnimPlay", Vec2(35.f, 20.f)))
		{
			m_bPlay = true;
			m_iCurIdx = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop##AnimStop", Vec2(35.f, 20.f)))
		{
			m_bPlay = false;
			m_AccTime = 0;
			m_iCurIdx = 0;
		}

		//ImGui::PushItemWidth(100.f);

		ImGui::Text("Fps     "); ImGui::SameLine();
		AnimUIFps = (int)(1 / m_copyvector[m_iCurIdx].fDuration);
		ImGui::InputInt("##frameduration", &AnimUIFps);

		Animfullsize[0] = m_copyvector[m_iCurIdx].vFullSize.x * fWidth;
		Animfullsize[1] = m_copyvector[m_iCurIdx].vFullSize.y * fHeight;

		ImGui::Text("FullSize"); ImGui::SameLine(); ImGui::InputFloat2("##AnimFullSize", Animfullsize);

		Animlefttop[0] = m_copyvector[0].vLeftTop.x * fWidth;
		Animlefttop[1] = m_copyvector[0].vLeftTop.y * fHeight;
		ImGui::Text("LeftTop "); ImGui::SameLine(); ImGui::InputFloat2("##AnimLefttop", Animlefttop);
		if (!m_bPlay)
		{
			ImGui::SameLine();
			if (ImGui::Button("##texturecursor", Vec2(15.f, 15.f)))
			{
				m_iCurIdx = 0;
				AnimCursorUI* pAnimCursorUI = dynamic_cast<AnimCursorUI*>(CImGuiMgr::GetInst()->FindUI("AnimCursorUI"));
				assert(pAnimCursorUI);
				pAnimCursorUI->setTexture(m_AtlasTex);
				pAnimCursorUI->setOwner(this);
				pAnimCursorUI->setAnimSlice(AnimSlice);
				pAnimCursorUI->Open();
			}
		}

		Animoffset[0] = m_copyvector[m_iCurIdx].vOffset.x * fWidth;
		Animoffset[1] = m_copyvector[m_iCurIdx].vOffset.y * fHeight;
		ImGui::Text("Offset  "); ImGui::SameLine(); ImGui::InputFloat2("##Animoffset", Animoffset);

		AnimSlice[0] = m_copyvector[m_iCurIdx].vSlice.x * fWidth;
		AnimSlice[1] = m_copyvector[m_iCurIdx].vSlice.y * fHeight;
		ImGui::Text("Slice   "); ImGui::SameLine(); ImGui::InputFloat2("##Animslice", AnimSlice);
		ImGui::Text("Step    "); ImGui::SameLine(); ImGui::InputFloat("##Animstep", &AnimStep);

		if (!m_bPlay)
		{
			for (size_t j = 0; j < m_copyvector.size(); ++j)
			{
				if (AnimUIFps != 0)
				{
					m_copyvector[j].fDuration = 1.f / (float)AnimUIFps;
					m_copyvector[j].vFullSize = Vec2(Animfullsize[0] / fWidth, Animfullsize[1] / fHeight);
					m_copyvector[j].vOffset = Vec2(Animoffset[0] / fWidth, Animoffset[1] / fHeight);
					m_copyvector[j].vSlice = Vec2(AnimSlice[0] / fWidth, AnimSlice[1] / fHeight);
					m_copyvector[j].vLeftTop = Vec2((Animlefttop[0] + AnimStep * j) / fWidth, Animlefttop[1] / fHeight);

				}
			}
		}

		if (m_copyvector[m_iCurIdx].vSlice.x > m_copyvector[m_iCurIdx].vFullSize.x)
		{
			Animfullsize[0] = m_copyvector[m_iCurIdx].vSlice.x * fWidth;
		}

		if (m_copyvector[m_iCurIdx].vSlice.y > m_copyvector[m_iCurIdx].vFullSize.y)
		{
			Animfullsize[1] = m_copyvector[m_iCurIdx].vSlice.y * fHeight;
		}
	}

	//ImGui::PopItemWidth();

	ImGui::Separator();
	ImGui::Text("Frame   ");
	ImGui::SameLine();
	if (ImGui::Button("Add##AnimAdd", Vec2(35.f, 20.f)))
	{
		tAnim2DFrm frm = {};
		m_copyvector.insert(m_copyvector.begin() + m_iCurIdx + 1, frm);
	}
	ImGui::SameLine();
	if (ImGui::Button("Del##AnimDel", Vec2(35.f, 20.f)))
	{
		m_copyvector.erase(m_copyvector.begin() + m_iCurIdx);
		if (m_iCurIdx == m_copyvector.size() && m_iCurIdx != 0)
			--m_iCurIdx;
	}
	if (!m_bPlay)
	{
		for (size_t j = 0; j < m_copyvector.size(); ++j)
		{
			if (AnimUIFps != 0)
			{
				m_copyvector[j].fDuration = 1.f / (float)AnimUIFps;
				m_copyvector[j].vFullSize = Vec2(Animfullsize[0] / fWidth, Animfullsize[1] / fHeight);
			}
		}
	}

	ImGui::Separator();
	if (ImGui::Button("Save##AnimSave", Vec2(70.f, 20.f)))
	{
		owner->setzero();
		m_Animation->GetFrmVec().clear();
		vector<tAnim2DFrm>::iterator iter = m_copyvector.begin();
		for (; iter != m_copyvector.end(); ++iter)
		{
			m_Animation->GetFrmVec().push_back(*iter);
		}
		m_Animation->SetAnimAtlas(m_AtlasTex);
		m_Animation->Reset();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel##AnimCancel", Vec2(70.f, 20.f)))
	{
		Close();
	}
}

void AnimUI::Close()
{
	UI::Close();

	m_Animation = nullptr;
	m_AtlasTex = nullptr;
	m_AccTime = 0.f;
	m_iCurIdx = 0;
	m_copyvector.clear();
	m_bPlay = false;
	m_bStepAnimation = false;
	AnimStep = 0.f;
	ResetFocus();
}

void AnimUI::setVector(vector<tAnim2DFrm> _copyvec)
{
	vector<tAnim2DFrm>::iterator iter = _copyvec.begin();
	for (; iter != _copyvec.end(); ++iter)
	{
		m_copyvector.push_back(*iter);
	}
}