#include "pch.h"
#include "CTileMap.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"


CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_bDataChanged(false)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));

	m_TileBuffer = new CStructuredBuffer;
}

CTileMap::~CTileMap()
{
	if (nullptr != m_TileBuffer)
		delete m_TileBuffer;
}

void CTileMap::SetTileCount(UINT _iWidth, UINT _iHeight)
{
	m_vTileCount = Vec2((float)_iWidth, (float)_iHeight);

	m_vecTile.clear();
	m_vecTile.resize(_iWidth * _iHeight);

	if ( m_TileBuffer->GetElementCount() < _iWidth * _iHeight)
	{
		delete(m_TileBuffer);
		m_TileBuffer = new CStructuredBuffer;
		m_TileBuffer->Create(sizeof(tTile), _iWidth * _iHeight, SB_TYPE::SRV_ONLY, nullptr, true);
	}

	//// Ÿ�� ������ ���� ����
	//float fWidth = (float)m_AtlasTex->GetWidth();
	//float fHeight = (float)m_AtlasTex->GetHeight();

	//Vec2 vLeftTop = Vec2(64.f, 64.f);
	//Vec2 vSlice = Vec2(64.f, 64.f);

	//vLeftTop /= Vec2(fWidth, fHeight);
	//vSlice /= Vec2(fWidth, fHeight);

	//for (size_t i = 0; i < m_vecTile.size(); ++i)
	//{
	//	m_vecTile[i].vLeftTop = vLeftTop;
	//	m_vecTile[i].vSlice = vSlice;
	//}

	//m_vecTile[0].vLeftTop = Vec2(0.f, 0.f);

	//m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));
}

void CTileMap::SetTile(UINT _index, tTile _info)
{
	m_bDataChanged = true;
	if (m_vecTile.max_size() <= _index)
	{
		assert(0);
	}
	m_vecTile[_index] = _info;
}

void CTileMap::finaltick()
{
	
}

void CTileMap::render()
{
	Transform()->UpdateData();
	
	if (m_bDataChanged)
	{
		m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));
	}	
	m_TileBuffer->UpdateData(17, PIPELINE_STAGE::PS);

	float fWidth = (float)m_AtlasTex->GetWidth();
	float fHeight = (float)m_AtlasTex->GetHeight();

	Vec2 vLeftTop = Vec2(64.f, 64.f);
	Vec2 vSlice = Vec2(64.f, 64.f);

	GetCurMaterial()->SetScalarParam(VEC2_0, &vLeftTop);
	GetCurMaterial()->SetScalarParam(VEC2_1, &vSlice);
	GetCurMaterial()->SetScalarParam(VEC2_2, &m_vTileCount);

	GetCurMaterial()->SetTexParam(TEX_0, m_AtlasTex);

	GetCurMaterial()->UpdateData();

	GetMesh()->render();
}



void CTileMap::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	SaveResourceRef<CTexture>(m_AtlasTex, _File);

	fwrite(&m_vTileCount, sizeof(Vec2), 1, _File);
	fwrite(m_vecTile.data(), sizeof(tTile), m_vecTile.size(), _File);	
	fwrite(&m_vSlice, sizeof(Vec2), 1, _File);
}

void CTileMap::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	// ���� ��Ʋ�� �ؽ��� ����
	LoadResourceRef<CTexture>(m_AtlasTex, _File);

	// Ÿ�� ���� Ȯ�� ��, ���� �������� �� ����ȭ���� �޸� ����
	fread(&m_vTileCount, sizeof(Vec2), 1, _File);
	size_t iTileCount = (size_t)(m_vTileCount.x * m_vTileCount.y);
	SetTileCount((UINT)m_vTileCount.x, (UINT)m_vTileCount.y);

	// ���Ϸκ��� Ÿ�� ���� �о ���Ϳ� ���
	for (size_t i = 0; i < iTileCount; ++i)
	{
		tTile tile = {};
		fread(&tile, sizeof(tTile), 1, _File);
		m_vecTile[i] = tile;
	}
	
	// Ÿ�������� 1�� �̻� ������, ������ ������ üũ --> render ���� �����͸� ����ȭ���۷� �̵�
	if (0 < m_vecTile.size())
		m_bDataChanged = true;
	fread(&m_vSlice, sizeof(Vec2), 1, _File);
}