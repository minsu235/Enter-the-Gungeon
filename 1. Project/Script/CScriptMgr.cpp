#include "pch.h"
#include "CScriptMgr.h"

#include "CBlankScript.h"
#include "CBooklletScript.h"
#include "CBulletKingScript.h"
#include "CBulletKinScript.h"
#include "CBulletScript.h"
#include "CButtonScript.h"
#include "CChestScript.h"
#include "CDoorScript.h"
#include "CEffectScript.h"
#include "CENVScript.h"
#include "CGameCameraScript.h"
#include "CGunnutScript.h"
#include "CGunScript.h"
#include "CItemScript.h"
#include "CMonsterArm.h"
#include "CMonsterBulletScript.h"
#include "CMonsterGun.h"
#include "CPlayerArmScript.h"
#include "CPlayerScript.h"
#include "CRoomScript.h"
#include "CShopItemScript.h"
#include "CShotgunBulletKinScript.h"
#include "CSpawnScript.h"
#include "CTableScript.h"
#include "CUIScript.h"
#include "CWallScript.h"
#include "CWingScript.h"
#include "CWizardBulletScript.h"
#include "CWizardScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBlankScript");
	_vec.push_back(L"CBooklletScript");
	_vec.push_back(L"CBulletKingScript");
	_vec.push_back(L"CBulletKinScript");
	_vec.push_back(L"CBulletScript");
	_vec.push_back(L"CButtonScript");
	_vec.push_back(L"CChestScript");
	_vec.push_back(L"CDoorScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CENVScript");
	_vec.push_back(L"CGameCameraScript");
	_vec.push_back(L"CGunnutScript");
	_vec.push_back(L"CGunScript");
	_vec.push_back(L"CItemScript");
	_vec.push_back(L"CMonsterArm");
	_vec.push_back(L"CMonsterBulletScript");
	_vec.push_back(L"CMonsterGun");
	_vec.push_back(L"CPlayerArmScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CRoomScript");
	_vec.push_back(L"CShopItemScript");
	_vec.push_back(L"CShotgunBulletKinScript");
	_vec.push_back(L"CSpawnScript");
	_vec.push_back(L"CTableScript");
	_vec.push_back(L"CUIScript");
	_vec.push_back(L"CWallScript");
	_vec.push_back(L"CWingScript");
	_vec.push_back(L"CWizardBulletScript");
	_vec.push_back(L"CWizardScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBlankScript" == _strScriptName)
		return new CBlankScript;
	if (L"CBooklletScript" == _strScriptName)
		return new CBooklletScript;
	if (L"CBulletKingScript" == _strScriptName)
		return new CBulletKingScript;
	if (L"CBulletKinScript" == _strScriptName)
		return new CBulletKinScript;
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CButtonScript" == _strScriptName)
		return new CButtonScript;
	if (L"CChestScript" == _strScriptName)
		return new CChestScript;
	if (L"CDoorScript" == _strScriptName)
		return new CDoorScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CENVScript" == _strScriptName)
		return new CENVScript;
	if (L"CGameCameraScript" == _strScriptName)
		return new CGameCameraScript;
	if (L"CGunnutScript" == _strScriptName)
		return new CGunnutScript;
	if (L"CGunScript" == _strScriptName)
		return new CGunScript;
	if (L"CItemScript" == _strScriptName)
		return new CItemScript;
	if (L"CMonsterArm" == _strScriptName)
		return new CMonsterArm;
	if (L"CMonsterBulletScript" == _strScriptName)
		return new CMonsterBulletScript;
	if (L"CMonsterGun" == _strScriptName)
		return new CMonsterGun;
	if (L"CPlayerArmScript" == _strScriptName)
		return new CPlayerArmScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CRoomScript" == _strScriptName)
		return new CRoomScript;
	if (L"CShopItemScript" == _strScriptName)
		return new CShopItemScript;
	if (L"CShotgunBulletKinScript" == _strScriptName)
		return new CShotgunBulletKinScript;
	if (L"CSpawnScript" == _strScriptName)
		return new CSpawnScript;
	if (L"CTableScript" == _strScriptName)
		return new CTableScript;
	if (L"CUIScript" == _strScriptName)
		return new CUIScript;
	if (L"CWallScript" == _strScriptName)
		return new CWallScript;
	if (L"CWingScript" == _strScriptName)
		return new CWingScript;
	if (L"CWizardBulletScript" == _strScriptName)
		return new CWizardBulletScript;
	if (L"CWizardScript" == _strScriptName)
		return new CWizardScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BLANKSCRIPT:
		return new CBlankScript;
		break;
	case (UINT)SCRIPT_TYPE::BOOKLLETSCRIPT:
		return new CBooklletScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETKINGSCRIPT:
		return new CBulletKingScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETKINSCRIPT:
		return new CBulletKinScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::BUTTONSCRIPT:
		return new CButtonScript;
		break;
	case (UINT)SCRIPT_TYPE::CHESTSCRIPT:
		return new CChestScript;
		break;
	case (UINT)SCRIPT_TYPE::DOORSCRIPT:
		return new CDoorScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::ENVSCRIPT:
		return new CENVScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMECAMERASCRIPT:
		return new CGameCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::GUNNUTSCRIPT:
		return new CGunnutScript;
		break;
	case (UINT)SCRIPT_TYPE::GUNSCRIPT:
		return new CGunScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMSCRIPT:
		return new CItemScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERARM:
		return new CMonsterArm;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERBULLETSCRIPT:
		return new CMonsterBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERGUN:
		return new CMonsterGun;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERARMSCRIPT:
		return new CPlayerArmScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::ROOMSCRIPT:
		return new CRoomScript;
		break;
	case (UINT)SCRIPT_TYPE::SHOPITEMSCRIPT:
		return new CShopItemScript;
		break;
	case (UINT)SCRIPT_TYPE::SHOTGUNBULLETKINSCRIPT:
		return new CShotgunBulletKinScript;
		break;
	case (UINT)SCRIPT_TYPE::SPAWNSCRIPT:
		return new CSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::TABLESCRIPT:
		return new CTableScript;
		break;
	case (UINT)SCRIPT_TYPE::UISCRIPT:
		return new CUIScript;
		break;
	case (UINT)SCRIPT_TYPE::WALLSCRIPT:
		return new CWallScript;
		break;
	case (UINT)SCRIPT_TYPE::WINGSCRIPT:
		return new CWingScript;
		break;
	case (UINT)SCRIPT_TYPE::WIZARDBULLETSCRIPT:
		return new CWizardBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::WIZARDSCRIPT:
		return new CWizardScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BLANKSCRIPT:
		return L"CBlankScript";
		break;

	case SCRIPT_TYPE::BOOKLLETSCRIPT:
		return L"CBooklletScript";
		break;

	case SCRIPT_TYPE::BULLETKINGSCRIPT:
		return L"CBulletKingScript";
		break;

	case SCRIPT_TYPE::BULLETKINSCRIPT:
		return L"CBulletKinScript";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::BUTTONSCRIPT:
		return L"CButtonScript";
		break;

	case SCRIPT_TYPE::CHESTSCRIPT:
		return L"CChestScript";
		break;

	case SCRIPT_TYPE::DOORSCRIPT:
		return L"CDoorScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::ENVSCRIPT:
		return L"CENVScript";
		break;

	case SCRIPT_TYPE::GAMECAMERASCRIPT:
		return L"CGameCameraScript";
		break;

	case SCRIPT_TYPE::GUNNUTSCRIPT:
		return L"CGunnutScript";
		break;

	case SCRIPT_TYPE::GUNSCRIPT:
		return L"CGunScript";
		break;

	case SCRIPT_TYPE::ITEMSCRIPT:
		return L"CItemScript";
		break;

	case SCRIPT_TYPE::MONSTERARM:
		return L"CMonsterArm";
		break;

	case SCRIPT_TYPE::MONSTERBULLETSCRIPT:
		return L"CMonsterBulletScript";
		break;

	case SCRIPT_TYPE::MONSTERGUN:
		return L"CMonsterGun";
		break;

	case SCRIPT_TYPE::PLAYERARMSCRIPT:
		return L"CPlayerArmScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::ROOMSCRIPT:
		return L"CRoomScript";
		break;

	case SCRIPT_TYPE::SHOPITEMSCRIPT:
		return L"CShopItemScript";
		break;

	case SCRIPT_TYPE::SHOTGUNBULLETKINSCRIPT:
		return L"CShotgunBulletKinScript";
		break;

	case SCRIPT_TYPE::SPAWNSCRIPT:
		return L"CSpawnScript";
		break;

	case SCRIPT_TYPE::TABLESCRIPT:
		return L"CTableScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return L"CUIScript";
		break;

	case SCRIPT_TYPE::WALLSCRIPT:
		return L"CWallScript";
		break;

	case SCRIPT_TYPE::WINGSCRIPT:
		return L"CWingScript";
		break;

	case SCRIPT_TYPE::WIZARDBULLETSCRIPT:
		return L"CWizardBulletScript";
		break;

	case SCRIPT_TYPE::WIZARDSCRIPT:
		return L"CWizardScript";
		break;

	}
	return nullptr;
}