#pragma once


class CLevel;
class CGameObject;
class CComponent;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
private:
	CLevel*			m_pCurLevel;
	Vec3			m_mousePos;
	CSound*			m_pBGM;

	CLevel*			m_pLevel;
	bool			m_debug;
public:
	CLevel* GetCurLevel() { return m_pCurLevel; }
	LEVEL_STATE GetLevelState();
	void playBGM(const wstring& _strKey);
	void setLevel(CLevel* _level) { m_pLevel = _level; }
	void LevelChange() { ChangeLevel(m_pLevel); m_pLevel = nullptr; }
	void setDebug(bool _bool) { m_debug = _bool; }
	bool IsDebug() { return m_debug; }

public:
	void init();
	void progress();	
	
public:
	CGameObject* FindObjectByName(const wstring& _strName);
	void FindObjectByName(const wstring& _strName, vector<CGameObject*>& _out);

	void ChangeLevel(CLevel* _NextLevel);
	Vec3 getMousepos() { return m_mousePos; }

	void CreateStageLevel();
	void CreateTitleLevel();
private:
	CGameObject* LoadTilemap(wstring _strRelativePath);
	CGameObject* LoadGameObject(FILE* _File);
	CComponent* LoadAnimator2D(wstring _strRelativePath);
	
private:
	void ChangeLevelState(LEVEL_STATE _State);

public:
	CLevelMgr();
	~CLevelMgr();

	friend class CEventMgr;
};

