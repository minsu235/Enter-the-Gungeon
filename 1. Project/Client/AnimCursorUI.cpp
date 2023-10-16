#include "pch.h"
#include "AnimCursorUI.h"

#include "AnimUI.h"
#include "ListUI.h"
#include "CImGuiMgr.h"
#include <Engine/CResMgr.h>

AnimCursorUI::AnimCursorUI()
	: UI("AnimCursorUI")
	, owner(nullptr)
	, AnimSlice{}
	, m_check(false)
{
	Close();
}

AnimCursorUI::~AnimCursorUI()
{
}

void AnimCursorUI::update()
{
	UI::update();
}

void AnimCursorUI::render_update()
{
	string TexName;
	if (nullptr != m_AtlasTex)
	{
		TexName = string(m_AtlasTex->GetKey().begin(), m_AtlasTex->GetKey().end());
	}

	ImGui::Text("Texture    ");
	ImGui::SameLine();
	ImGui::InputText("##TexName_AnimcursorUI", (char*)TexName.data(), TexName.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##TexBtn_AnimcursorUI", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메쉬 목록을 받아와서, ListUI 에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&AnimCursorUI::SetOtherTexture);
		ImGui::SetNextWindowFocus();
		pListUI->Open();
	}
	ImGui::Text("Auto Slice"); ImGui::SameLine(); ImGui::Checkbox("##Autoslice", &m_check);

	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2((float)m_AtlasTex->GetWidth(), (float)m_AtlasTex->GetHeight()));

	if (!m_check)
	{
		if (ImGui::IsItemHovered())
		{
			/*ImGuiIO& io = ImGui::GetIO();
			float my_tex_w = m_AtlasTex->GetWidth() / 2;
			float my_tex_h = m_AtlasTex->GetHeight() / 2;
			ImGui::BeginTooltip();
			float region_sz_x = AnimSlice[0] / 2;
			float region_sz_y = AnimSlice[1] / 2;
			float region_x = io.MousePos.x - pos.x;
			float region_y = io.MousePos.y - pos.y;
			float zoom = 1.2f;
			if (region_x < 0.0f) { region_x = 0.0f; }
			else if (region_x > my_tex_w - region_sz_x) { region_x = my_tex_w - region_sz_x; }
			if (region_y < 0.0f) { region_y = 0.0f; }
			else if (region_y > my_tex_h - region_sz_y) { region_y = my_tex_h - region_sz_y; }
			ImGui::Text("Min: (%.2f, %.2f)", region_x * 2, region_y * 2);
			ImGui::Text("Max: (%.2f, %.2f)", (region_x + region_sz_x) * 2, (region_y + region_sz_y) * 2);
			ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
			ImVec2 uv1 = ImVec2((region_x + region_sz_x) / my_tex_w, (region_y + region_sz_y) / my_tex_h);
			ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2(region_sz_x * zoom, region_sz_y * zoom), uv0, uv1);
			ImGui::EndTooltip();


			if (ImGui::IsMouseDown(0))
			{
				float a[2] = { region_x * 2 , region_y * 2 };
				owner->setAnimlefttop(a);
			}*/

			ImGuiIO& io = ImGui::GetIO();
			float my_tex_w = (float)m_AtlasTex->GetWidth();
			float my_tex_h = (float)m_AtlasTex->GetHeight();
			ImGui::BeginTooltip();
			float region_sz_x = AnimSlice[0];
			float region_sz_y = AnimSlice[1];
			float region_x = io.MousePos.x - pos.x;
			float region_y = io.MousePos.y - pos.y;
			float zoom = 1.f;
			if (region_x < 0.0f) { region_x = 0.0f; }
			else if (region_x > my_tex_w - region_sz_x) { region_x = my_tex_w - region_sz_x; }
			if (region_y < 0.0f) { region_y = 0.0f; }
			else if (region_y > my_tex_h - region_sz_y) { region_y = my_tex_h - region_sz_y; }
			ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
			ImGui::Text("Max: (%.2f, %.2f)", (region_x + region_sz_x), (region_y + region_sz_y));
			ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
			ImVec2 uv1 = ImVec2((region_x + region_sz_x) / my_tex_w, (region_y + region_sz_y) / my_tex_h);
			ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2(region_sz_x * zoom, region_sz_y * zoom), uv0, uv1);
			ImGui::EndTooltip();


			if (ImGui::IsMouseDown(0))
			{
				float a[2] = { region_x , region_y };
				owner->setAnimlefttop(a);
			}

			Vec4 a = GetRGBA((int)region_x, (int)region_y);
			ImGui::Text("%f, %f, %f, %f", a.x, a.y, a.z, a.w);
		}
	}
	else
	{
		if (ImGui::IsItemHovered())
		{
			ImGuiIO& io = ImGui::GetIO();
			float region_x = io.MousePos.x - pos.x;
			float region_y = io.MousePos.y - pos.y;
			ImGui::BeginTooltip();
			Vec4 a = GetRGBA((int)region_x, (int)region_y);
			ImGui::Text("RGBA : %f, %f, %f, %f", a.x, a.y, a.z, a.w);
			ImGui::EndTooltip();

			if (ImGui::IsMouseDown(0))
			{
				int pImageX = m_AtlasTex->GetWidth();
				int pImageY = m_AtlasTex->GetHeight();
				int row[] = { -1, 0, 0, 1 };
				int col[] = { 0, 1, -1, 0 };
				int top = (int)region_y, bottom = (int)region_y, left = (int)region_x, right = (int)region_x;
				queue<Vec2> que;
				que.push(Vec2(region_x, region_y));
				map<int, int> map;
				map.insert(make_pair(((int)region_y * pImageX + (int)region_x), 0));

				while (!que.empty())
				{
					Vec2 pixel = que.front();
					que.pop();
					if (GetRGBA((int)pixel.x, (int)pixel.y).w != 0.f)
					{
						if (top > (int)pixel.y)
							top = (int)pixel.y;
						if (bottom < (int)pixel.y)
							bottom = (int)pixel.y;
						if (left > (int)pixel.x)
							left = (int)pixel.x;
						if (right < (int)pixel.x)
							right = (int)pixel.x;

						for (int k = 0; k < 4; k++)
						{
							if ((int)(pixel.y + col[k]) < 0 || (int)(pixel.y + col[k]) >= pImageY)
								continue;
							if ((int)pixel.x + row[k] < 0 || (int)pixel.x + row[k] >= pImageX)
								continue;
							if (map.find((int)(pixel.y + col[k]) * pImageX + (int)pixel.x + row[k]) == map.end())
							{
								que.push(Vec2((float)(pixel.x + row[k]), (float)(pixel.y + col[k])));
								map.insert(make_pair(((int)(pixel.y + col[k]) * pImageX + (int)(pixel.x + row[k])), 0));
							}
						}
					}
				}

				float lefttop[2] = { (float)left , (float)top };
				owner->setAnimlefttop(lefttop);
				float slice[2] = { (float)(right - left), (float)(bottom - top) };
				owner->setAnimSlice(slice);
			}
		}
	}


}

void AnimCursorUI::Close()
{
	UI::Close();
	m_AtlasTex = nullptr;
	owner = nullptr;
	m_check = false;
}

Vec4 AnimCursorUI::GetRGBA(int x, int y)
{
	int pImageX = m_AtlasTex->GetWidth();
	int pImageY = m_AtlasTex->GetHeight();
	ScratchImage& pImage = m_AtlasTex->GetImage();
	uint8_t* pixeldata = pImage.GetPixels();
	int idx = (y * pImageX + x) * 4;
	char d = pixeldata[idx];
	return Vec4((float)pixeldata[idx + 2], (float)pixeldata[idx + 1], (float)pixeldata[idx], (float)pixeldata[idx + 3]);
}

void AnimCursorUI::SetOtherTexture(DWORD_PTR _strTexKey)
{
	string strKey = (char*)_strTexKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);
	assert(nullptr != pTex);

	owner->setTexture(pTex);
	m_AtlasTex = pTex;
}
