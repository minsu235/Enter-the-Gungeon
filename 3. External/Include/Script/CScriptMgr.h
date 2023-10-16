#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BLANKSCRIPT,
	BOOKLLETSCRIPT,
	BULLETKINGSCRIPT,
	BULLETKINSCRIPT,
	BULLETSCRIPT,
	BUTTONSCRIPT,
	CHESTSCRIPT,
	DOORSCRIPT,
	EFFECTSCRIPT,
	ENVSCRIPT,
	GAMECAMERASCRIPT,
	GUNNUTSCRIPT,
	GUNSCRIPT,
	ITEMSCRIPT,
	MONSTERARM,
	MONSTERBULLETSCRIPT,
	MONSTERGUN,
	PLAYERARMSCRIPT,
	PLAYERSCRIPT,
	ROOMSCRIPT,
	SHOPITEMSCRIPT,
	SHOTGUNBULLETKINSCRIPT,
	SPAWNSCRIPT,
	TABLESCRIPT,
	UISCRIPT,
	WALLSCRIPT,
	WINGSCRIPT,
	WIZARDBULLETSCRIPT,
	WIZARDSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
