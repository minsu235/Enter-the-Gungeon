#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D*               m_pCurAnim;

    bool                        m_bRepeat;

public:    
    virtual void begin() override;
    virtual void finaltick() override;

public:
    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    CAnimation2D* FindAnimation(const wstring& _strKey);
    const map<wstring, CAnimation2D*>& GetMapAnimation() { return m_mapAnim; }

    CAnimation2D* GetCurrentAnimation() { return m_pCurAnim; }
    void Play(const wstring& _strKey, bool _bRepeat);
    void UpdateData();
    void Clear();

public:
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
    CLONE(CAnimator2D);

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _other);
    ~CAnimator2D();
};

