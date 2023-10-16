#pragma once
#include "CRenderComponent.h"

#include "Ptr.h"
#include "CTexture.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>       m_AtlasTex;     // Ÿ�ϸ��� ����ϴ� ��Ʋ�� �̹���
    Vec2                m_vTileCount;   // Ÿ�ϸ� ���� ���� ����
    Vec2                m_vSlice;

    vector<tTile>       m_vecTile;
    CStructuredBuffer*  m_TileBuffer;   // �� Ÿ���� ��Ʋ�� �������� ����ü

    bool                m_bDataChanged;

public:
    void SetTileAtlas(Ptr<CTexture> _AtlasTex) { m_AtlasTex = _AtlasTex; }
    Ptr<CTexture> GetTileAtlas() { return m_AtlasTex; }
    void SetTileCount(UINT _iWidth, UINT _iHeight);
    Vec2 GetTileCount() { return m_vTileCount; }
    void SetTileSlice(Vec2 _vSlice) { m_vSlice = _vSlice; }
    Vec2 GetTileSlice() { return m_vSlice; }
    void SetTile(UINT _index, tTile _info);


public:
    virtual void finaltick() override;
    virtual void render() override;


public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CTileMap);

public:
    CTileMap();
    ~CTileMap();
};

