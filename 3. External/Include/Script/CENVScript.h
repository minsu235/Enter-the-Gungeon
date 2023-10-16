#pragma once
#include <Engine\CScript.h>

enum class ENV_Type
{
    BOX,
    BARREL,
    SCONCE,
    BOOK,
    lich,
};

class CENVScript :
    public CScript
{
private:
    bool m_bFirst;
    bool m_bBreak;
    ENV_Type m_eType;

public:
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CENVScript);

public:
    CENVScript(ENV_Type _type = ENV_Type::BOX);
    ~CENVScript();
};

