#pragma once
#include <Engine\CScript.h>

class Compare
{
public:
    bool operator() (tNode* _pLeft, tNode* _pRight)
    {
        return _pLeft->fFinal > _pRight->fFinal;
    }
};

enum class Monster
{
    Bookllet,
    BulletKin,
    Gunnut,
    ShotgunKinRed,
    ShotgunKinBlue,
    Wizard,
    Wall_h,
    Wall_v,
    BulletKing,
};

enum class RoomType
{
    None,
    Monster,
    Boss,
    Shop,
};
struct MonsterInfo
{
    Monster m_eMonster;
    int     m_iWave;
    Vec3    m_vPos;
};

class CRoomScript :
    public CScript
{
public:
    tNode                                           arrNode[100][100];
private:
    CGameObject*                                    m_pPlayer;
    priority_queue<tNode*, vector<tNode*>, Compare>	m_OpenList;
    int                                             m_iDestPosX;
    int                                             m_iDestPosY;
    int                                             m_iCurPosX;
    int                                             m_iCurPosY;
    int                                             m_iXCount;
    int                                             m_iYCount;
    CGameObject*                                    m_pDarkroom;
    vector<MonsterInfo>                             vec_monster;
    size_t                                          m_wavesize;
    int                                             m_iMonsterwave;
    int                                             m_iMonster;
    CGameObject*                                    m_Wall;
    CGameObject*                                    m_Wall2;
    bool                                            m_bSpawn;
    float                                           m_fAcctime;

    RoomType                                        m_eType;
public:
    Vec2 FindPath(CGameObject* _monster);
    Vec2 FindTele();
    void setDarkroom(CGameObject* _darkroom) { m_pDarkroom = _darkroom; }
    void addMonster(MonsterInfo _monster) { vec_monster.push_back(_monster); }
    void MonsterDie() { --m_iMonster; if (m_iMonster < 0) m_iMonster = 0; }
    void setType(RoomType _type) { m_eType = _type; }
private:
    void Rebuild(priority_queue<tNode*, vector<tNode*>, Compare>& _queue);
    void CalculateCost(tNode* _pCurNode, tNode* _pOrigin);
    void AddOpenList(int _iXIdx, int _iYIdx, tNode* _pOrigin);

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CRoomScript);

public:
    CRoomScript();
    ~CRoomScript();
};


