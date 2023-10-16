#include "pch.h"
#include "TileUI.h"
#include <Engine\CDevice.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <Engine\CTransform.h>
#include <Engine\CTileMap.h>

TileUI::TileUI()
	: UI("TileUI")
	, TileTargetIdx(-1)
	, mousetileIdx(-1)
{
	Close();
	SetSize(ImVec2(300.f, 500.f));
}

TileUI::~TileUI()
{
}

void TileUI::render_update()
{
	//m_AtlasTex
	//m_vTileCount
	//m_vSlice

	Vec2 TextureTileCount = Vec2(m_AtlasTex->GetWidth() / m_vSlice.x, m_AtlasTex->GetHeight() / m_vSlice.y);
	for (int i = 0; i < (int)(TextureTileCount.x * TextureTileCount.y); ++i)
	{
		ImGui::PushID(i);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
		ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
		ImVec2 uv0 = ImVec2(1.f / TextureTileCount.x * (i % (int)TextureTileCount.x), 1.f / TextureTileCount.y * (i / (int)TextureTileCount.x));                            // UV coordinates for lower-left
		ImVec2 uv1 = ImVec2(1.f / TextureTileCount.x * (i % (int)TextureTileCount.x) + 1.f / TextureTileCount.x, 1.f / TextureTileCount.y * (i / (int)TextureTileCount.x) + 1.f / TextureTileCount.y);    // UV coordinates for (32,32) in our texture
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
		if (ImGui::ImageButton("", (void*)m_AtlasTex->GetSRV().Get(), size, uv0, uv1, bg_col, tint_col))
		{
			TileTargetIdx = i;
		}
		ImGui::PopStyleVar();
		ImGui::PopID();
		if (i % (int)TextureTileCount.x != (int)TextureTileCount.x - 1)
		{
			ImGui::SameLine();
		}
	}
	ImGui::NewLine();
	ImGui::Separator();

	Vec2 vRes = CDevice::GetInst()->GetRenderResolution();
	Vec3 vPos = { CKeyMgr::GetInst()->GetMousePos().x, CKeyMgr::GetInst()->GetMousePos().y, 0.f };
	vPos.y = vRes.y - vPos.y;
	vPos.x -= vRes.x / 2;
	vPos.y -= vRes.y / 2;
	vPos.y /= vRes.y / 2;
	vPos.x /= vRes.x / 2;

	Matrix vCam = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();
	vCam = XMMatrixInverse(nullptr, vCam);
	vPos = XMVector3TransformNormal(vPos, vCam);

	vCam = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
	vCam = XMMatrixInverse(nullptr, vCam);
	vPos = XMVector3TransformCoord(vPos, vCam);
	Vec2 LeftTop = Vec2(m_Target->Transform()->GetWorldPos().x - m_Target->Transform()->GetRelativeScale().x / 2.f,
		m_Target->Transform()->GetWorldPos().y + m_Target->Transform()->GetRelativeScale().y / 2.f);

	ImGui::Text("MousePos    : x - %f y - %f", vPos.x, vPos.y);
	vPos.x -= LeftTop.x;
	vPos.y -= LeftTop.y;
	vPos.y *= -1;
	Vec2 tilesize = Vec2(m_Target->Transform()->GetRelativeScale().x / m_vTileCount.x, m_Target->Transform()->GetRelativeScale().y / m_vTileCount.y);
	if (vPos.x < 0 || vPos.x > m_Target->Transform()->GetRelativeScale().x
		|| vPos.y < 0 || vPos.y > m_Target->Transform()->GetRelativeScale().y)
		mousetileIdx = -1;
	else
	{
		mousetileIdx = (int)(vPos.y / tilesize.y) * (int)m_vTileCount.x + (int)(vPos.x / tilesize.x);
	}
	ImGui::Text("TileIdx    : %d", mousetileIdx);
	ImGui::Text("SelectTile : %d", TileTargetIdx);
}

void TileUI::update()
{
	UI::update();

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		if (mousetileIdx != -1 && TileTargetIdx != -1)
		{
			Vec2 TextureTileCount = Vec2(m_AtlasTex->GetWidth() / m_vSlice.x, m_AtlasTex->GetHeight() / m_vSlice.y);

			tTile info = {};
			info.vSlice = Vec2(m_vSlice.x / m_AtlasTex->GetWidth(), m_vSlice.y / m_AtlasTex->GetHeight());
			info.vLeftTop = Vec2(1.f / TextureTileCount.x * (TileTargetIdx % (int)TextureTileCount.x), 1.f / TextureTileCount.y * (TileTargetIdx / (int)TextureTileCount.x));
			m_Target->TileMap()->SetTile((UINT)mousetileIdx, info);
		}
	}
}

void TileUI::Open()
{
	UI::Open();
}

void TileUI::Close()
{
	UI::Close();

	m_AtlasTex = nullptr;
	m_vTileCount = Vec2(0.f, 0.f);
	m_vSlice = Vec2(0.f, 0.f);
	TileTargetIdx = -1;
	ResetFocus();
}


