#include "pch.h"
#include "CreateTestLevel.h"

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CComponent.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\GlobalComponent.h>

#include <Engine\CRenderMgr.h>
#include <Engine\CPaintShader.h>

#include <Script\CScriptMgr.h>
#include <Script\CPlayerScript.h>
#include <Script\CGunScript.h>
#include <Script/CGameCameraScript.h>
#include <Script/CBulletScript.h>
#include <Script/CPlayerArmScript.h>
#include <Script/CBooklletScript.h>
#include <Script/CMonsterBulletScript.h>
#include <Script/CRoomScript.h>
#include <Script/CBlankScript.h>
#include <Script/CUIScript.h>
#include <Script/CEffectScript.h>
#include <Script/CWizardScript.h>
#include <Script/CGunnutScript.h>
#include <Script/CShotgunBulletKinScript.h>
#include <Script/CMonsterArm.h>
#include <Script/CMonsterGun.h>
#include <Script/CBulletKingScript.h>
#include <Script/CBulletKinScript.h>
#include <Script/CChestScript.h>
#include <Script/CWingScript.h>
#include <Script/CTableScript.h>
#include <Script/CENVScript.h>
#include <Script/CDoorScript.h>
#include <Script/CWallScript.h>
#include <Script/CShopItemScript.h>
#include <Script/CButtonScript.h>

#include "CSaveLoadMgr.h"


void CreateStageLevel()
{	
	CLevel* pLevel = new CLevel;
	pLevel->SetName(L"Stage");

	// Layer 이름 설정
	pLevel->GetLayer(1)->SetName(L"Player");
	pLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(3)->SetName(L"Monster");
	pLevel->GetLayer(4)->SetName(L"MonsterProjectile");
	pLevel->GetLayer(5)->SetName(L"Map");
	pLevel->GetLayer(6)->SetName(L"Item");
	pLevel->GetLayer(7)->SetName(L"Object");
	pLevel->GetLayer(31)->SetName(L"UI");


	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CGameCameraScript);

	pCamObj->Camera()->SetLayerMaskAll();
	pCamObj->Camera()->SetLayerMask(31);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);

	pCamObj->Transform()->SetRelativePos(Vec3(-1967.f, -633.f, 0.f));

	CRenderMgr::GetInst()->RegisterMainGameCamera(pCamObj->Camera());
	pLevel->AddGameObject(pCamObj, 0);

	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");

	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);

	pUICam->Camera()->SetLayerMask(31);
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);

	pLevel->AddGameObject(pUICam, 0);

	// Directional Light 추가
	CGameObject* pDirLight = new CGameObject;
	pDirLight->SetName(L"DirectionalLight");

	pDirLight->AddComponent(new CTransform);
	pDirLight->AddComponent(new CLight2D);

	pDirLight->Light2D()->SetLightColor(Vec3(0.85f, 0.85f, 0.85f));
	pDirLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

	pLevel->AddGameObject(pDirLight, 0);


	// PointLight 추가
	CGameObject* pPointLight = new CGameObject;
	pPointLight->SetName(L"Light");

	pPointLight->AddComponent(new CTransform);
	pPointLight->AddComponent(new CLight2D);

	pPointLight->Transform()->SetRelativePos(4320.f, 2900.f, 0.f);

	pPointLight->Light2D()->SetLightColor(Vec3(0.5f, 0.4f, 0.2f));
	pPointLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pPointLight->Light2D()->SetRadius(400.f);

	pLevel->AddGameObject(pPointLight, 0);

	pPointLight = new CGameObject;
	pPointLight->SetName(L"Light");

	pPointLight->AddComponent(new CTransform);
	pPointLight->AddComponent(new CLight2D);

	pPointLight->Transform()->SetRelativePos(3750.f, 2400.f, 0.f);

	pPointLight->Light2D()->SetLightColor(Vec3(0.5f, 0.4f, 0.2f));
	pPointLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pPointLight->Light2D()->SetRadius(400.f);

	pLevel->AddGameObject(pPointLight, 0);

	pPointLight = new CGameObject;
	pPointLight->SetName(L"Light");

	pPointLight->AddComponent(new CTransform);
	pPointLight->AddComponent(new CLight2D);

	pPointLight->Transform()->SetRelativePos(4890.f, 2400.f, 0.f);

	pPointLight->Light2D()->SetLightColor(Vec3(0.5f, 0.4f, 0.2f));
	pPointLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pPointLight->Light2D()->SetRadius(400.f);

	pLevel->AddGameObject(pPointLight, 0);

	CGameObject* pDoor = new CGameObject;
	pDoor->SetName(L"Door_right");
	pDoor->AddComponent(new CTransform);
	pDoor->AddComponent(new CMeshRender);
	pDoor->AddComponent(new CCollider2D);
	pDoor->AddComponent(LoadAnimator2D(L"animation\\door_right.anim"));
	pDoor->AddComponent(new CDoorScript);

	pDoor->Transform()->SetRelativeScale(Vec3(60.f, 210.f, 0.f));

	pDoor->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pDoor->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pDoor->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	Ptr<CPrefab> Doorright = new CPrefab(pDoor, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"door_right", Doorright.Get());

	pDoor = new CGameObject;
	pDoor->SetName(L"Door_up");
	pDoor->AddComponent(new CTransform);
	pDoor->AddComponent(new CMeshRender);
	pDoor->AddComponent(new CCollider2D);
	pDoor->AddComponent(LoadAnimator2D(L"animation\\door_up.anim"));
	pDoor->AddComponent(new CDoorScript);

	pDoor->Transform()->SetRelativeScale(Vec3(102.f, 129.f, 0.f));

	pDoor->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pDoor->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pDoor->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	Ptr<CPrefab> Doorup = new CPrefab(pDoor, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"door_up", Doorup.Get());

	pDoor = new CGameObject;
	pDoor->SetName(L"Door_down");
	pDoor->AddComponent(new CTransform);
	pDoor->AddComponent(new CMeshRender);
	pDoor->AddComponent(new CCollider2D);
	pDoor->AddComponent(LoadAnimator2D(L"animation\\door_down.anim"));
	pDoor->AddComponent(new CDoorScript);

	pDoor->Transform()->SetRelativeScale(Vec3(102.f, 144.f, 0.f));

	pDoor->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pDoor->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pDoor->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	Ptr<CPrefab> Doordown = new CPrefab(pDoor, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"door_down", Doordown.Get());

	CGameObject* pfire = new CGameObject;
	pfire->AddComponent(new CTransform);
	pfire->AddComponent(new CMeshRender);
	pfire->AddComponent(LoadAnimator2D(L"animation\\cartoonfire.anim"));
	pfire->Transform()->SetRelativePos(Vec3(-17.f, 12.f, 0.f));
	pfire->Transform()->SetRelativeScale(Vec3(42.f, 54.f, 0.f));
	pfire->Transform()->SetIgnoreParentScale(true);
	pfire->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pfire->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	Ptr<CPrefab> fire = new CPrefab(pfire, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"cartoonfire", fire.Get());

	pDoor = new CGameObject;
	pDoor->SetName(L"boss_door");
	pDoor->AddComponent(new CTransform);
	pDoor->AddComponent(new CMeshRender);
	pDoor->AddComponent(new CCollider2D);
	pDoor->AddComponent(LoadAnimator2D(L"animation\\bossdoor.anim"));
	pDoor->AddComponent(new CDoorScript);

	pDoor->Transform()->SetRelativeScale(Vec3(102.f, 225.f, 0.f));

	pDoor->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pDoor->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pDoor->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pDoor->Collider2D()->SetIgnoreObjectScale(true);
	pDoor->Collider2D()->SetScale(Vec2(144.f, 192.f));
	pDoor->Collider2D()->SetOffsetPos(Vec2(-120.f, -88.f));
	pDoor->GetScript<CDoorScript>()->setBossdoor();
	Ptr<CPrefab> bossdoor = new CPrefab(pDoor, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"boss_door", bossdoor.Get());

	pDoor = new CGameObject;
	pDoor->SetName(L"wall_door");
	pDoor->AddComponent(new CTransform);
	pDoor->AddComponent(new CMeshRender);
	pDoor->AddComponent(new CCollider2D);
	pDoor->AddComponent(new CWallScript);

	pDoor->Transform()->SetRelativeScale(Vec3(96.f, 137.f, 76.f));

	pDoor->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pDoor->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"wall_door_vMtrl"));
	pDoor->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\castle_door_block_vertical_headshake_001.png"));

	pDoor->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);

	Ptr<CPrefab> walldoor = new CPrefab(pDoor, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"walldoor_v", walldoor.Get());

	pDoor = new CGameObject;
	pDoor->SetName(L"wall_door");
	pDoor->AddComponent(new CTransform);
	pDoor->AddComponent(new CMeshRender);
	pDoor->AddComponent(new CCollider2D);
	pDoor->AddComponent(new CWallScript);

	pDoor->Transform()->SetRelativeScale(Vec3(18.f, 153.f, 76.f));

	pDoor->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pDoor->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"wall_door_hMtrl"));
	pDoor->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\castle_door_block_horizontal_up_001.png"));

	pDoor->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);

	walldoor = new CPrefab(pDoor, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"walldoor_h", walldoor.Get());

	CGameObject* pEnv = new CGameObject;
	pEnv->SetName(L"ENV");
	pEnv->AddComponent(new CTransform);
	pEnv->AddComponent(new CMeshRender);
	pEnv->AddComponent(new CCollider2D);
	pEnv->AddComponent(LoadAnimator2D(L"animation\\box.anim"));
	pEnv->AddComponent(new CENVScript);

	pEnv->Transform()->SetRelativePos(Vec3(0.f, 800.f, 75.f));
	pEnv->Transform()->SetRelativeScale(Vec3(65.f, 65.f, 0.f));

	pEnv->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEnv->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pEnv->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pEnv->Collider2D()->SetIgnoreObjectScale(true);
	pEnv->Collider2D()->SetScale(Vec2(45.f, 55.f));
	Ptr<CPrefab> boxprefab = new CPrefab(pEnv, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"box", boxprefab.Get());


	pEnv = new CGameObject;
	pEnv->SetName(L"ENV");
	pEnv->AddComponent(new CTransform);
	pEnv->AddComponent(new CMeshRender);
	pEnv->AddComponent(new CCollider2D);
	pEnv->AddComponent(LoadAnimator2D(L"animation\\barrel.anim"));
	pEnv->AddComponent(new CENVScript(ENV_Type::BARREL));

	pEnv->Transform()->SetRelativePos(Vec3(0.f, 850.f, 75.f));
	pEnv->Transform()->SetRelativeScale(Vec3(70.f, 70.f, 0.f));

	pEnv->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEnv->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pEnv->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pEnv->Collider2D()->SetIgnoreObjectScale(true);
	pEnv->Collider2D()->SetScale(Vec2(45.f, 60.f));
	Ptr<CPrefab> barrel = new CPrefab(pEnv, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"barrel", barrel.Get());

	pEnv = new CGameObject;
	pEnv->SetName(L"ENV");
	pEnv->AddComponent(new CTransform);
	pEnv->AddComponent(new CMeshRender);
	pEnv->AddComponent(LoadAnimator2D(L"animation\\sconce_leftwall.anim"));
	pEnv->AddComponent(new CENVScript(ENV_Type::SCONCE));

	pEnv->Transform()->SetRelativeScale(Vec3(33.f, 51.f, 0.f));

	pEnv->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEnv->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	CGameObject* pENVLight = new CGameObject;
	pENVLight->SetName(L"PointLight");

	pENVLight->AddComponent(new CTransform);
	pENVLight->AddComponent(new CLight2D);

	pENVLight->Transform()->SetRelativePos(0.f, 0.f, 0.f);

	pENVLight->Light2D()->SetLightColor(Vec3(1.f, 0.9f, 0.6f));
	pENVLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pENVLight->Light2D()->SetRadius(300.f);

	pEnv->AddChild(pENVLight);
	Ptr<CPrefab> leftLightprefab = new CPrefab(pEnv, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"sconce_leftwall", leftLightprefab.Get());

	pEnv = new CGameObject;
	pEnv->SetName(L"ENV");
	pEnv->AddComponent(new CTransform);
	pEnv->AddComponent(new CMeshRender);
	pEnv->AddComponent(LoadAnimator2D(L"animation\\sconce_rightwall.anim"));
	pEnv->AddComponent(new CENVScript(ENV_Type::SCONCE));

	pEnv->Transform()->SetRelativeScale(Vec3(33.f, 51.f, 0.f));

	pEnv->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEnv->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pENVLight = new CGameObject;
	pENVLight->SetName(L"PointLight");

	pENVLight->AddComponent(new CTransform);
	pENVLight->AddComponent(new CLight2D);

	pENVLight->Transform()->SetRelativePos(0.f, 0.f, 0.f);

	pENVLight->Light2D()->SetLightColor(Vec3(1.f, 0.9f, 0.6f));
	pENVLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pENVLight->Light2D()->SetRadius(300.f);

	pEnv->AddChild(pENVLight);
	Ptr<CPrefab> rightLightprefab = new CPrefab(pEnv, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"sconce_rightwall", rightLightprefab.Get());

	pEnv = new CGameObject;
	pEnv->SetName(L"ENV");
	pEnv->AddComponent(new CTransform);
	pEnv->AddComponent(new CMeshRender);
	pEnv->AddComponent(LoadAnimator2D(L"animation\\sconce_backwall.anim"));
	pEnv->AddComponent(new CENVScript(ENV_Type::SCONCE));

	pEnv->Transform()->SetRelativeScale(Vec3(33.f, 51.f, 0.f));

	pEnv->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEnv->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pENVLight = new CGameObject;
	pENVLight->SetName(L"PointLight");

	pENVLight->AddComponent(new CTransform);
	pENVLight->AddComponent(new CLight2D);

	pENVLight->Transform()->SetRelativePos(0.f, 0.f, 0.f);

	pENVLight->Light2D()->SetLightColor(Vec3(1.f, 0.9f, 0.6f));
	pENVLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pENVLight->Light2D()->SetRadius(300.f);

	pEnv->AddChild(pENVLight);
	Ptr<CPrefab> Lightprefab = new CPrefab(pEnv, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"sconce_backwall", Lightprefab.Get());

	CGameObject* pTable = new CGameObject;
	pTable->SetName(L"Table");
	pTable->AddComponent(new CTransform);
	pTable->AddComponent(new CMeshRender);
	pTable->AddComponent(new CCollider2D);
	pTable->AddComponent(LoadAnimator2D(L"animation\\table.anim"));
	pTable->AddComponent(new CTableScript);

	pTable->Transform()->SetRelativeScale(Vec3(135.f, 207.f, 0.f));

	pTable->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTable->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pTable->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pTable->Collider2D()->SetIgnoreObjectScale(true);
	pTable->Collider2D()->SetScale(Vec2(140.f, 120.f));
	Ptr<CPrefab> tableprefab = new CPrefab(pTable, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"table", tableprefab.Get());

	// PostProcess Object
	CGameObject* pPostProcess = new CGameObject;
	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);
	pPostProcess->AddComponent(new CBlankScript);
	pPostProcess->AddComponent(new CCollider2D);

	pPostProcess->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 1.f));
	pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 10.f));

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	Ptr<CPrefab> prefab = new CPrefab(pPostProcess, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Blank", prefab.Get());

	pPostProcess = new CGameObject;
	pPostProcess->SetName(L"Darkroom");
	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativeScale(Vec3(700.f, 700.f, 1.f));

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DarkroomMtrl"));

	Ptr<CPrefab> darkroom = new CPrefab(pPostProcess, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Darkroom", darkroom.Get());
	{
		pLevel->AddGameObject(LoadTilemap(L"map\\start_wall.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\start_child.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\start_shadow.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\start_front.map"), 5);
	}
	{
		CGameObject* ptile = LoadTilemap(L"map\\tableroom_wall.map");
		CGameObject* dark = darkroom->Instantiate();
		dark->Transform()->SetRelativeScale(Vec3(1152.f, 1300.f, 1.f));
		Instantiate(dark, Vec3(2304.f, 1632.f, 75.f), 0, true, pLevel);
		ptile->GetScript<CRoomScript>()->setDarkroom(dark);
		ptile->GetScript<CRoomScript>()->setType(RoomType::Monster);
		MonsterInfo info;
		info.m_eMonster = Monster::Wall_h;
		info.m_iWave = 0;
		info.m_vPos = Vec3(1778.f, 1300.f, 74.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Wall_v;
		info.m_iWave = 0;
		info.m_vPos = Vec3(2640.f, 2085.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::ShotgunKinBlue;
		info.m_iWave = 0;
		info.m_vPos = Vec3(2330.f, 1475.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Gunnut;
		info.m_iWave = 1;
		info.m_vPos = Vec3(2330.f, 1475.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Bookllet;
		info.m_iWave = 1;
		info.m_vPos = Vec3(1965.f, 1972.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Bookllet;
		info.m_iWave = 1;
		info.m_vPos = Vec3(2689.f, 1800.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Wizard;
		info.m_iWave = 1;
		info.m_vPos = Vec3(2012.f, 1316.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::ShotgunKinBlue;
		info.m_iWave = 2;
		info.m_vPos = Vec3(2255.f, 1600.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::ShotgunKinRed;
		info.m_iWave = 2;
		info.m_vPos = Vec3(2255.f, 1350.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::ShotgunKinBlue;
		info.m_iWave = 2;
		info.m_vPos = Vec3(2445.f, 1470.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		pLevel->AddGameObject(ptile, 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\tableroom_child.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\tableroom_shadow.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\tableroom_front.map"), 5);
		Instantiate(leftLightprefab->Instantiate(), Vec3(1888.f, 1700.f, 75.f), 0, true, pLevel);
		Instantiate(rightLightprefab->Instantiate(), Vec3(2768.f, 1400.f, 75.f), 0, true, pLevel);
		Instantiate(tableprefab->Instantiate(), Vec3(2520.f, 1300.f, 75.f), 7, true, pLevel);
		Instantiate(tableprefab->Instantiate(), Vec3(2520.f, 1550.f, 75.f), 7, true, pLevel);
		Instantiate(tableprefab->Instantiate(), Vec3(2130.f, 1300.f, 75.f), 7, true, pLevel);
		Instantiate(tableprefab->Instantiate(), Vec3(2130.f, 1550.f, 75.f), 7, true, pLevel);
		Instantiate(Doorright->Instantiate(), Vec3(1800.f, 1290.f, 75.f), 8, true, pLevel);
		Instantiate(Doorup->Instantiate(), Vec3(2640.f, 2100.f, 75.f), 8, true, pLevel);
	}

	{
		CGameObject* ptile = LoadTilemap(L"map\\Test1_wall.map");
		CGameObject* dark = darkroom->Instantiate();
		dark->Transform()->SetRelativeScale(Vec3(1056.f, 1300.f, 1.f));
		Instantiate(dark, Vec3(1200.f, 1344.f, 75.f), 0, true, pLevel);
		ptile->GetScript<CRoomScript>()->setDarkroom(dark);
		pLevel->AddGameObject(ptile, 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\Test1_child.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\Test1_shadow.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\Test1_front.map"), 5);
		Instantiate(Doorright->Instantiate(), Vec3(1620.f, 1290.f, 75.f), 8, true, pLevel);
		Instantiate(Doorright->Instantiate(), Vec3(770.f, 1290.f, 75.f), 8, true, pLevel);

		CGameObject* pWing = new CGameObject;
		pWing->SetName(L"Wing");
		pWing->AddComponent(new CTransform);
		pWing->AddComponent(new CMeshRender);
		pWing->AddComponent(LoadAnimator2D(L"animation\\wing.anim"));
		pWing->AddComponent(new CWingScript);

		pWing->Transform()->SetRelativeScale(Vec3(87.f, 57.f, 0.f));

		pWing->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pWing->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
		Ptr<CPrefab> prefab1 = new CPrefab(pWing, true);
		CResMgr::GetInst()->AddRes<CPrefab>(L"wing", prefab1.Get());

		CGameObject* pChest = new CGameObject;
		pChest->SetName(L"Chest");
		pChest->AddComponent(new CTransform);
		pChest->AddComponent(new CMeshRender);
		pChest->AddComponent(new CCollider2D);
		pChest->AddComponent(LoadAnimator2D(L"animation\\chest.anim"));
		pChest->AddComponent(new CChestScript(ItemCode::shotgun));

		pChest->Transform()->SetRelativePos(Vec3(1054.f, 1320.f, 75.f));
		pChest->Transform()->SetRelativeScale(Vec3(108.f, 96.f, 0.f));

		pChest->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pChest->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

		pChest->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pChest->Collider2D()->SetIgnoreObjectScale(true);
		pChest->Collider2D()->SetScale(Vec2(140.f, 120.f));

		CGameObject* pChestwall = new CGameObject;
		pChestwall->SetName(L"Wall");
		pChestwall->AddComponent(new CTransform);
		pChestwall->AddComponent(new CCollider2D);
		pChestwall->Transform()->SetRelativePos(Vec3(0.f, -5.f, 0.f));
		pChestwall->Transform()->SetRelativeScale(Vec3(100.f, 80.f, 0.f));
		pChestwall->Transform()->SetIgnoreParentScale(true);

		pChest->AddChild(pChestwall);
		pLevel->AddGameObject(pChest, 6);

		pChest = new CGameObject;
		pChest->SetName(L"Chest");
		pChest->AddComponent(new CTransform);
		pChest->AddComponent(new CMeshRender);
		pChest->AddComponent(new CCollider2D);
		pChest->AddComponent(LoadAnimator2D(L"animation\\chest.anim"));
		pChest->AddComponent(new CChestScript(ItemCode::waxwing));

		pChest->Transform()->SetRelativePos(Vec3(1342.f, 1320.f, 75.f));//2000.f, -200.f, 75.f));
		pChest->Transform()->SetRelativeScale(Vec3(108.f, 96.f, 0.f));

		pChest->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pChest->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

		pChest->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pChest->Collider2D()->SetIgnoreObjectScale(true);
		pChest->Collider2D()->SetScale(Vec2(140.f, 120.f));

		pChestwall = new CGameObject;
		pChestwall->SetName(L"Wall");
		pChestwall->AddComponent(new CTransform);
		pChestwall->AddComponent(new CCollider2D);
		pChestwall->Transform()->SetRelativePos(Vec3(0.f, -5.f, 0.f));//2000.f, -200.f, 75.f));
		pChestwall->Transform()->SetRelativeScale(Vec3(100.f, 80.f, 0.f));
		pChestwall->Transform()->SetIgnoreParentScale(true);

		pChest->AddChild(pChestwall);
		pLevel->AddGameObject(pChest, 6);
	}

	{
		CGameObject* ptile = LoadTilemap(L"map\\way_wall.map");
		CGameObject* dark = darkroom->Instantiate();
		dark->Transform()->SetRelativeScale(Vec3(432.f, 960.f, 1.f));
		Instantiate(dark, Vec3(1248.f, 432.f, 75.f), 0, true, pLevel);
		ptile->GetScript<CRoomScript>()->setDarkroom(dark);
		pLevel->AddGameObject(ptile, 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\way_child.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\way_shadow.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\way_front.map"), 5);
		Instantiate(Doordown->Instantiate(), Vec3(1200.f, 950.f, 75.f), 8, true, pLevel);
	}

	{
		CGameObject* ptile = LoadTilemap(L"map\\shop_wall.map");
		CGameObject* dark = darkroom->Instantiate();
		dark->Transform()->SetRelativeScale(Vec3(1200.f, 1056.f, 1.f));
		Instantiate(dark, Vec3(1992.f, 384.f, 75.f), 0, true, pLevel);
		ptile->GetScript<CRoomScript>()->setDarkroom(dark);
		ptile->GetScript<CRoomScript>()->setType(RoomType::Shop);
		pLevel->AddGameObject(ptile, 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\shop_shadow.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\shop_front.map"), 5);
		Instantiate(Lightprefab->Instantiate(), Vec3(2250.f, 730.f, 75.f), 0, true, pLevel);
		Instantiate(Lightprefab->Instantiate(), Vec3(1605.f, 635.f, 75.f), 0, true, pLevel);
		Instantiate(leftLightprefab->Instantiate(), Vec3(1552.f, 210.f, 75.f), 0, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2368.f, 208.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2368.f, 256.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2368.f, 304.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2368.f, 352.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2368.f, 400.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2368.f, 448.f, 75.f), 5, true, pLevel);
		Instantiate(boxprefab->Instantiate(), Vec3(1568.f, 180.f, 75.f), 5, true, pLevel);
		Instantiate(boxprefab->Instantiate(), Vec3(1568.f, 250.f, 75.f), 5, true, pLevel);
		Instantiate(Doorright->Instantiate(), Vec3(1430.f, 90.f, 75.f), 8, true, pLevel);

		CGameObject* pSellItem = new CGameObject;
		pSellItem->SetName(L"Megahand");
		pSellItem->AddComponent(new CTransform);
		pSellItem->AddComponent(new CMeshRender);
		pSellItem->AddComponent(new CCollider2D);
		pSellItem->AddComponent(new CShopItemScript(ItemCode::megahand));

		pSellItem->Transform()->SetRelativeScale(Vec3(30.f, 25.f, 0.f));

		pSellItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pSellItem->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemMegahandMtrl"));
		pSellItem->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\mega_buster_idle_001.png"));

		pSellItem->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pSellItem->Collider2D()->SetIgnoreObjectScale(true);
		pSellItem->Collider2D()->SetScale(Vec2(140.f, 20.f));
		Instantiate(pSellItem, Vec3(2075.f, 610.f, 74.f), 6, true, pLevel);

		pSellItem = new CGameObject;
		pSellItem->SetName(L"Scope");
		pSellItem->AddComponent(new CTransform);
		pSellItem->AddComponent(new CMeshRender);
		pSellItem->AddComponent(new CCollider2D);
		pSellItem->AddComponent(new CShopItemScript(ItemCode::scope));

		pSellItem->Transform()->SetRelativeScale(Vec3(42.f, 34.f, 0.f));
		pSellItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pSellItem->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemScopeMtrl"));
		pSellItem->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\scope_001.png"));

		pSellItem->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pSellItem->Collider2D()->SetIgnoreObjectScale(true);
		pSellItem->Collider2D()->SetScale(Vec2(140.f, 20.f));
		Instantiate(pSellItem, Vec3(2075.f, 540.f, 74.f), 6, true, pLevel);

		pSellItem = new CGameObject;
		pSellItem->SetName(L"Blank");
		pSellItem->AddComponent(new CTransform);
		pSellItem->AddComponent(new CMeshRender);
		pSellItem->AddComponent(new CCollider2D);
		pSellItem->AddComponent(new CShopItemScript(ItemCode::blank));

		pSellItem->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 0.f));

		pSellItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pSellItem->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"itemblankMtrl"));

		pSellItem->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pSellItem->Collider2D()->SetIgnoreObjectScale(true);
		pSellItem->Collider2D()->SetScale(Vec2(20.f, 240.f));
		Instantiate(pSellItem, Vec3(1650.f, 430.f, 74.f), 6, true, pLevel);

		pSellItem = new CGameObject;
		pSellItem->SetName(L"Blank");
		pSellItem->AddComponent(new CTransform);
		pSellItem->AddComponent(new CMeshRender);
		pSellItem->AddComponent(new CCollider2D);
		pSellItem->AddComponent(new CShopItemScript(ItemCode::blank));

		pSellItem->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 0.f));

		pSellItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pSellItem->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"itemblankMtrl"));

		pSellItem->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pSellItem->Collider2D()->SetIgnoreObjectScale(true);
		pSellItem->Collider2D()->SetScale(Vec2(20.f, 240.f));
		Instantiate(pSellItem, Vec3(1730.f, 430.f, 74.f), 6, true, pLevel);

		pSellItem = new CGameObject;
		pSellItem->SetName(L"Heart");
		pSellItem->AddComponent(new CTransform);
		pSellItem->AddComponent(new CMeshRender);
		pSellItem->AddComponent(new CCollider2D);
		pSellItem->AddComponent(new CShopItemScript(ItemCode::heart));

		pSellItem->Transform()->SetRelativeScale(Vec3(48.f, 48.f, 0.f));

		pSellItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pSellItem->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"itemheartMtrl"));

		pSellItem->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pSellItem->Collider2D()->SetIgnoreObjectScale(true);
		pSellItem->Collider2D()->SetScale(Vec2(20.f, 240.f));
		Instantiate(pSellItem, Vec3(1910.f, 430.f, 74.f), 6, true, pLevel);

		pSellItem = new CGameObject;
		pSellItem->SetName(L"Heart");
		pSellItem->AddComponent(new CTransform);
		pSellItem->AddComponent(new CMeshRender);
		pSellItem->AddComponent(new CCollider2D);
		pSellItem->AddComponent(new CShopItemScript(ItemCode::heart));

		pSellItem->Transform()->SetRelativeScale(Vec3(48.f, 48.f, 0.f));

		pSellItem->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pSellItem->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"itemheartMtrl"));

		pSellItem->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
		pSellItem->Collider2D()->SetIgnoreObjectScale(true);
		pSellItem->Collider2D()->SetScale(Vec2(20.f, 240.f));
		Instantiate(pSellItem, Vec3(1990.f, 430.f, 74.f), 6, true, pLevel);
	}

	{
		CGameObject* ptile = LoadTilemap(L"map\\Test2_wall.map");
		CGameObject* dark = darkroom->Instantiate();
		dark->Transform()->SetRelativeScale(Vec3(1344.f, 1344.f, 1.f));
		Instantiate(dark, Vec3(0.f, 1104.f, 75.f), 0, true, pLevel);
		ptile->GetScript<CRoomScript>()->setDarkroom(dark);
		ptile->GetScript<CRoomScript>()->setType(RoomType::Monster);
		MonsterInfo info;
		info.m_eMonster = Monster::Wall_h;
		info.m_iWave = 0;
		info.m_vPos = Vec3(534.f, 1300.f, 76.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Wall_v;
		info.m_iWave = 0;
		info.m_vPos = Vec3(0.f, 440.f, 74.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::BulletKin;
		info.m_iWave = 0;
		info.m_vPos = Vec3(0.f, 940.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::ShotgunKinBlue;
		info.m_iWave = 1;
		info.m_vPos = Vec3(150.f, 940.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::ShotgunKinRed;
		info.m_iWave = 1;
		info.m_vPos = Vec3(-150.f, 940.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Wizard;
		info.m_iWave = 2;
		info.m_vPos = Vec3(150.f, 940.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Wizard;
		info.m_iWave = 2;
		info.m_vPos = Vec3(-150.f, 940.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Bookllet;
		info.m_iWave = 3;
		info.m_vPos = Vec3(0.f, 940.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::Gunnut;
		info.m_iWave = 4;
		info.m_vPos = Vec3(0.f, 940.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		pLevel->AddGameObject(ptile, 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\Test2_shadow.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\Test2_front.map"), 5);
		Instantiate(leftLightprefab->Instantiate(), Vec3(-515.f, 850.f, 75.f), 0, true, pLevel);
		Instantiate(rightLightprefab->Instantiate(), Vec3(515.f, 950.f, 75.f), 0, true, pLevel);
		Instantiate(Lightprefab->Instantiate(), Vec3(200.f, 1605.f, 75.f), 0, true, pLevel);
		Instantiate(Lightprefab->Instantiate(), Vec3(-200.f, 1605.f, 75.f), 0, true, pLevel);
		Instantiate(Doorright->Instantiate(), Vec3(570.f, 1290.f, 75.f), 8, true, pLevel);
		Instantiate(Doorup->Instantiate(), Vec3(0.f, 510.f, 75.f), 8, true, pLevel);
	}

	{
		CGameObject* ptile = LoadTilemap(L"map\\dodge_wall.map");
		CGameObject* dark = darkroom->Instantiate();
		dark->Transform()->SetRelativeScale(Vec3(1632.f, 1632.f, 1.f));
		Instantiate(dark, Vec3(-624.f, -240.f, 75.f), 0, true, pLevel);
		ptile->GetScript<CRoomScript>()->setDarkroom(dark);
		pLevel->AddGameObject(ptile, 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\dodge_shadow.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\dodge_front.map"), 5);
		Instantiate(Lightprefab->Instantiate(), Vec3(-720.f, -470.f, 75.f), 0, true, pLevel);
		Instantiate(Lightprefab->Instantiate(), Vec3(-200.f, 200.f, 75.f), 0, true, pLevel);
		Instantiate(rightLightprefab->Instantiate(), Vec3(130.f, -400.f, 75.f), 0, true, pLevel);
		Instantiate(boxprefab->Instantiate(), Vec3(-696.f, -548.f, 75.f), 5, true, pLevel);
		Instantiate(boxprefab->Instantiate(), Vec3(-744.f, -548.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(110.f, -804.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(110.f, -756.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(110.f, -708.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(110.f, -660.f, 75.f), 5, true, pLevel);
		Instantiate(Doorup->Instantiate(), Vec3(0.f, 250.f, 75.f), 8, true, pLevel);
		Instantiate(Doorright->Instantiate(), Vec3(-1400.f, -677.f, 75.f), 8, true, pLevel);
	}

	{
		CGameObject* ptile = LoadTilemap(L"map\\bossdoor_wall.map");
		CGameObject* dark = darkroom->Instantiate();
		dark->Transform()->SetRelativeScale(Vec3(1300.f, 672.f, 1.f));
		Instantiate(dark, Vec3(2832.f, 2544.f, 75.f), 0, true, pLevel);
		ptile->GetScript<CRoomScript>()->setDarkroom(dark);
		pLevel->AddGameObject(ptile, 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\bossdoor_child.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\bossdoor_shadow.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\bossdoor_front.map"), 5);
		Instantiate(barrel->Instantiate(), Vec3(2525.f, 2672.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2525.f, 2720.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2573.f, 2720.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2621.f, 2720.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2669.f, 2720.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2799.f, 2720.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2847.f, 2720.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2895.f, 2720.f, 75.f), 5, true, pLevel);
		Instantiate(barrel->Instantiate(), Vec3(2943.f, 2720.f, 75.f), 5, true, pLevel);
		Instantiate(bossdoor->Instantiate(), Vec3(3026.f, 2585.f, 74.f), 8, true, pLevel);
	}

	{
		CGameObject* ptile = LoadTilemap(L"map\\Boss_wall.map");
		CGameObject* dark = darkroom->Instantiate();
		dark->Transform()->SetRelativeScale(Vec3(2400.f, 1700.f, 1.f));
		Instantiate(dark, Vec3(4272.f, 2736.f, 75.f), 0, true, pLevel);
		ptile->GetScript<CRoomScript>()->setDarkroom(dark);
		ptile->GetScript<CRoomScript>()->setType(RoomType::Boss);
		MonsterInfo info;
		info.m_eMonster = Monster::Wall_h;
		info.m_iWave = 0;
		info.m_vPos = Vec3(3160.f, 2550.f, 74.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		info.m_eMonster = Monster::BulletKing;
		info.m_iWave = 0;
		info.m_vPos = Vec3(4320.f, 2975.f, 75.f);
		ptile->GetScript<CRoomScript>()->addMonster(info);
		pLevel->AddGameObject(ptile, 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\Boss_child.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\Boss_shadow.map"), 5);
		pLevel->AddGameObject(LoadTilemap(L"map\\Boss_front.map"), 5);
		Instantiate(Lightprefab->Instantiate(), Vec3(3500.f, 2980.f, 75.f), 0, true, pLevel);
		Instantiate(Lightprefab->Instantiate(), Vec3(5140.f, 2980.f, 75.f), 0, true, pLevel);
		Instantiate(Lightprefab->Instantiate(), Vec3(3670.f, 3170.f, 75.f), 0, true, pLevel);
		Instantiate(Lightprefab->Instantiate(), Vec3(4965.f, 3170.f, 75.f), 0, true, pLevel);

	}
	
	//CGameObject* _tilemap = LoadTilemap(L"map\\Boss_wall.map");
	//pLevel->AddGameObject(_tilemap, 5);
	//pLevel->AddGameObject(LoadTilemap(L"map\\Boss_child.map"), 5);
	//pLevel->AddGameObject(LoadTilemap(L"map\\Boss_shadow.map"), 5);
	//pLevel->AddGameObject(LoadTilemap(L"map\\Boss_front.map"), 5);

	/*CGameObject* pTileMapObj = new CGameObject;

	pTileMapObj->SetName(L"Room");
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);
	pTileMapObj->AddComponent(new CCollider2D);
	pTileMapObj->AddComponent(new CRoomScript);

	pTileMapObj->Transform()->SetRelativePos(Vec3(2000.f, 0.f, 1000.f));
	pTileMapObj->Transform()->SetRelativeScale(Vec3(2112.f, 1440.f, 1.f));
	pTileMapObj->Transform()->SetIgnoreParentScale(true);


	pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\tileset_castle.png"));
	pTileMapObj->TileMap()->SetTileSlice(Vec2(16.f, 16.f));
	pTileMapObj->TileMap()->SetTileCount(44, 30);

	pLevel->AddGameObject(pTileMapObj, 1);

	pTileMapObj = new CGameObject;

	pTileMapObj->SetName(L"Room_child");
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);
	pTileMapObj->Transform()->SetIgnoreParentScale(true);

	pTileMapObj->Transform()->SetRelativePos(Vec3(2000.f, 0.f, 500.f));
	pTileMapObj->Transform()->SetRelativeScale(Vec3(2112.f, 1440.f, 1.f));

	pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\tileset_castle.png"));
	pTileMapObj->TileMap()->SetTileSlice(Vec2(16.f, 16.f));
	pTileMapObj->TileMap()->SetTileCount(44, 30);

	pLevel->AddGameObject(pTileMapObj, 5);

	pTileMapObj = new CGameObject;

	pTileMapObj->SetName(L"Room_shadow");
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);
	pTileMapObj->Transform()->SetIgnoreParentScale(true);

	pTileMapObj->Transform()->SetRelativePos(Vec3(2000.f, 0.f, 100.f));
	pTileMapObj->Transform()->SetRelativeScale(Vec3(2112.f, 1440.f, 1.f));

	pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\tileset_castle.png"));
	pTileMapObj->TileMap()->SetTileSlice(Vec2(16.f, 16.f));
	pTileMapObj->TileMap()->SetTileCount(44, 30);

	pLevel->AddGameObject(pTileMapObj, 5);

	pTileMapObj = new CGameObject;

	pTileMapObj->SetName(L"Room");
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);
	pTileMapObj->Transform()->SetIgnoreParentScale(true);

	pTileMapObj->Transform()->SetRelativePos(Vec3(2000.f, 0.f, 50.f));
	pTileMapObj->Transform()->SetRelativeScale(Vec3(2112.f, 1440.f, 1.f));

	pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\tileset_castle.png"));
	pTileMapObj->TileMap()->SetTileSlice(Vec2(16.f, 16.f));
	pTileMapObj->TileMap()->SetTileCount(44, 30);

	pLevel->AddGameObject(pTileMapObj, 5);*/

	// GameObject 초기화
	CGameObject* pObject = nullptr;

	pObject = new CGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(LoadAnimator2D(L"animation\\player.anim"));
	pObject->AddComponent(new CPlayerScript);
	

	pObject->Transform()->SetRelativePos(Vec3(-1967.f, -233.f, 75.f));//Vec3(2725.f, 2548.f, 75.f));
	pObject->Transform()->SetRelativeScale(Vec3(90.f, 105.f, 0.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pObject->Collider2D()->SetIgnoreObjectScale(true);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, -38.f));
	pObject->Collider2D()->SetScale(Vec2(48.f, 24.f));
	/*pObject->Animator2D()->CreateAnimation(L"Death", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f), Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Dodge_F", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f), Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Dodge_B", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f), Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Dodge_BR", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f), Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Dodge_BL", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f), Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Dodge_L", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f), Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Dodge_R", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"IDLE_B", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"IDLE_BR", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"IDLE_BL", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"IDLE_F", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"IDLE_L", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"IDLE_R", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"ITEMGET", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"FALL", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"FALL_Dodge", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f), Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"FALLback", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"RUN_B", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"RUN_BR", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"RUN_BL", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"RUN_F", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"RUN_L", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"RUN_R", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"SPIN", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),		Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"TABLE_BR", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"TABLE_BL", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"TABLE_R", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"TABLE_L", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"TABLE_FR", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"TABLE_FL", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Guide.png"), Vec2(0.f, 0.f),	Vec2(0.f, 0.f), 0.f, 10, 16);*/
	pObject->Animator2D()->Play(L"Death", true);

	pCamObj->GetScript<CGameCameraScript>()->setPlayer(pObject);

	CGameObject* pGun = new CGameObject;
	pGun->SetName(L"Gun");

	pGun->AddComponent(new CTransform);
	pGun->AddComponent(new CMeshRender);
	pGun->AddComponent(LoadAnimator2D(L"animation\\rust_sidearm.anim"));
	pGun->AddComponent(new CGunScript);


	pGun->Transform()->SetRelativePos(Vec3(20.f, 7.f, 0.f));
	pGun->Transform()->SetRelativeScale(Vec3(50.f, 47.f, 0.f));
	pGun->Transform()->SetIgnoreParentScale(true);

	pGun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pGun->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmMtrl"));
	pGun->GetScript<CGunScript>()->setGUN(GUN::rusty_sidearm);
	pGun->GetScript<CGunScript>()->setBoolact(true);

	pGun->Animator2D()->Play(L"IDLE", true);
	
	/*CGameObject* pGun = new CGameObject;
	pGun->SetName(L"Gun");

	pGun->AddComponent(new CTransform);
	pGun->AddComponent(new CMeshRender);
	pGun->AddComponent(LoadAnimator2D(L"animation\\shotgun.anim"));
	pGun->AddComponent(new CGunScript);

	pGun->Transform()->SetRelativePos(Vec3(20.f, 5.f, 0.f));
	pGun->Transform()->SetRelativeScale(Vec3(60.f, 40.f, 0.f));
	pGun->Transform()->SetIgnoreParentScale(true);

	pGun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pGun->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmMtrl"));
	pGun->GetScript<CGunScript>()->setGUN(GUN::shotgun);
	pGun->GetScript<CGunScript>()->setReloadtime(1.2f);
	pGun->GetScript<CGunScript>()->setBulletSpread(20.f);
	pGun->GetScript<CGunScript>()->setFireInterval(0.5f);
	pGun->Animator2D()->Play(L"IDLE", true);*/

	/*CGameObject* pGun = new CGameObject;
	pGun->SetName(L"Gun");

	pGun->AddComponent(new CTransform);
	pGun->AddComponent(new CMeshRender);
	pGun->AddComponent(LoadAnimator2D(L"animation\\megahand.anim"));
	pGun->AddComponent(new CGunScript);

	pGun->Transform()->SetRelativePos(Vec3(10.f, 0.f, 0.f));
	pGun->Transform()->SetRelativeScale(Vec3(44.f, 40.f, 0.f));
	pGun->Transform()->SetIgnoreParentScale(true);

	pGun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pGun->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmMtrl"));
	pGun->GetScript<CGunScript>()->setGUN(GUN::megahand);
	pGun->GetScript<CGunScript>()->setMagazineBullet(18);
	pGun->GetScript<CGunScript>()->setReloadtime(0.54f);
	pGun->GetScript<CGunScript>()->setFireInterval(0.07f);
	pGun->Animator2D()->Play(L"IDLE", true);*/

	CGameObject* pBullet = new CGameObject;
	pBullet->SetName(L"Bullet");
	pBullet->AddComponent(new CTransform);
	pBullet->AddComponent(new CMeshRender);
	pBullet->AddComponent(new CCollider2D);
	pBullet->AddComponent(new CBulletScript);

	pBullet->Transform()->SetRelativePos(Vec3(0.f, 0.f, 75.f));
	pBullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));

	pBullet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmBulletMtrl"));
	pBullet->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\player_Projectile.png"));
	pBullet->GetScript<CBulletScript>()->setGUN(GUN::rusty_sidearm);
	pBullet->GetScript<CBulletScript>()->setDuration(1.f);
	prefab = new CPrefab(pBullet, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"rust_sidearmBullet", prefab.Get());

	/*pBullet = new CGameObject;
	pBullet->SetName(L"Bullet");
	pBullet->AddComponent(new CTransform);
	pBullet->AddComponent(new CMeshRender);
	pBullet->AddComponent(new CCollider2D);
	pBullet->AddComponent(new CBulletScript);

	pBullet->Transform()->SetRelativePos(Vec3(0.f, 0.f, 75.f));
	pBullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));

	pBullet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmBulletMtrl"));
	pBullet->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\player_Projectile.png"));
	pBullet->GetScript<CBulletScript>()->setGUN(GUN::shotgun);
	pBullet->GetScript<CBulletScript>()->setDuration(0.5f);
	pBullet->GetScript<CBulletScript>()->setDMG(4);
	prefab = new CPrefab(pBullet, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"shotgunBullet", prefab.Get());*/

	//pBullet = new CGameObject;
	//pBullet->SetName(L"Bullet");
	//pBullet->AddComponent(new CTransform);
	//pBullet->AddComponent(new CMeshRender);
	//pBullet->AddComponent(new CCollider2D);
	//pBullet->AddComponent(LoadAnimator2D(L"animation\\bullet_megahand.anim"));
	//pBullet->AddComponent(new CBulletScript);
	//
	//pBullet->Transform()->SetRelativePos(Vec3(0.f, 0.f, 75.f));
	//pBullet->Transform()->SetRelativeScale(Vec3(18.f, 14.f, 0.f));
	//
	//pBullet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pBullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmBulletMtrl"));
	//pBullet->GetScript<CBulletScript>()->setGUN(GUN::megahand);
	//pBullet->GetScript<CBulletScript>()->setDMG(6);
	//pBullet->GetScript<CBulletScript>()->setSpeed(900.f);
	//prefab = new CPrefab(pBullet, true);
	//CResMgr::GetInst()->AddRes<CPrefab>(L"megahandBullet", prefab.Get());

	pGun->GetScript<CGunScript>()->setBullet(prefab);

	CGameObject* pPlayerArm = new CGameObject;
	pPlayerArm->SetName(L"PlayerArm");
	pPlayerArm->AddComponent(new CTransform);
	pPlayerArm->AddComponent(new CMeshRender);
	pPlayerArm->AddComponent(new CPlayerArmScript);

	pPlayerArm->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pPlayerArm->Transform()->SetRelativeScale(Vec3(12.f, 12.f, 0.f));
	pPlayerArm->Transform()->SetIgnoreParentScale(true);

	pPlayerArm->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPlayerArm->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PlayerArmMtrl"));
	pPlayerArm->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\guide_hand.png"));
	
	pPlayerArm->GetScript<CPlayerArmScript>()->setOffset(Vec3(0.f,-30.f,0.f));
	pPlayerArm->GetScript<CPlayerArmScript>()->setArmpos(Vec3(25.f, 0.f, 0.f));
	pPlayerArm->AddChild(pGun);
	pObject->AddChild(pPlayerArm);
	pLevel->AddGameObject(pObject, 1);

	CGameObject* pMonsterBullet = new CGameObject;
	pMonsterBullet->SetName(L"MonsterBullet");
	pMonsterBullet->AddComponent(new CTransform);
	pMonsterBullet->AddComponent(new CMeshRender);
	pMonsterBullet->AddComponent(new CCollider2D);
	pMonsterBullet->AddComponent(new CMonsterBulletScript);

	pMonsterBullet->Transform()->SetIgnoreParentScale(true);

	pMonsterBullet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterBullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MonsterBulletMtrl"));
	pMonsterBullet->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\enemy_projectile.png"));

	Ptr<CPrefab> pMonBulletPrefab = new CPrefab(pMonsterBullet, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"MonsterBullet", pMonBulletPrefab.Get());

	CGameObject* pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(LoadAnimator2D(L"animation\\bookllet.anim"));
	pMonster->AddComponent(new CRigidBody);
	pMonster->AddComponent(new CBooklletScript);

	pMonster->Transform()->SetRelativePos(Vec3(2300.f, 1500.f, 75.f));
	pMonster->Transform()->SetRelativeScale(Vec3(57.f, 69.f, 0.f));

	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BooklletMtrl"));

	pMonster->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pMonster->Collider2D()->SetIgnoreObjectScale(true);
	pMonster->Collider2D()->SetScale(Vec2(40.f, 40.f));

	Ptr<CPrefab> Monprefab = new CPrefab(pMonster, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Bookllet", Monprefab.Get());


	pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(LoadAnimator2D(L"animation\\yellow_wizard.anim"));
	pMonster->AddComponent(new CRigidBody);
	pMonster->AddComponent(new CWizardScript);

	pMonster->Transform()->SetRelativePos(Vec3(0.f, 600.f, 75.f));
	pMonster->Transform()->SetRelativeScale(Vec3(86.f, 78.f, 0.f));

	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"WizardMtrl"));

	pMonster->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pMonster->Collider2D()->SetIgnoreObjectScale(true);
	pMonster->Collider2D()->SetScale(Vec2(40.f, 40.f));

	Monprefab = new CPrefab(pMonster, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Wizard", Monprefab.Get());

	pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(LoadAnimator2D(L"animation\\gunnut.anim"));
	pMonster->AddComponent(new CRigidBody);
	pMonster->AddComponent(new CGunnutScript);

	pMonster->Transform()->SetRelativePos(Vec3(0.f, 600.f, 75.f));
	pMonster->Transform()->SetRelativeScale(Vec3(280.f, 320.f, 0.f));

	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GunnutMtrl"));

	pMonster->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pMonster->Collider2D()->SetIgnoreObjectScale(true);
	pMonster->Collider2D()->SetScale(Vec2(80.f, 100.f));
	pMonster->Collider2D()->SetOffsetPos(Vec2(10.f, -20.f));

	Monprefab = new CPrefab(pMonster, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"Gunnut", Monprefab.Get());

	pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(LoadAnimator2D(L"animation\\shotgunkin_red.anim"));
	pMonster->AddComponent(new CRigidBody);
	pMonster->AddComponent(new CShotgunBulletKinScript);

	pMonster->Transform()->SetRelativePos(Vec3(0.f, 800.f, 75.f));
	pMonster->Transform()->SetRelativeScale(Vec3(140.f, 120.f, 0.f));

	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ShotgunkinredMtrl"));

	
	pMonster->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pMonster->Collider2D()->SetIgnoreObjectScale(true);
	pMonster->Collider2D()->SetScale(Vec2(40.f, 55.f));
	pMonster->Collider2D()->SetOffsetPos(Vec2(0.f, -15.f));

	CGameObject* pMonsterGun = new CGameObject;
	pMonsterGun->SetName(L"MonsterGun");

	pMonsterGun->AddComponent(new CTransform);
	pMonsterGun->AddComponent(new CMeshRender);
	pMonsterGun->AddComponent(new CMonsterGun);

	pMonsterGun->Transform()->SetRelativePos(Vec3(20.f, 0.f, 0.f));
	pMonsterGun->Transform()->SetRelativeScale(Vec3(60.f, 15.f, 0.f));
	pMonsterGun->Transform()->SetIgnoreParentScale(true);

	pMonsterGun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterGun->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MonsterShotgunMtrl"));
	pMonsterGun->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\shotgun_idle_001.png"));
	pMonsterGun->GetScript<CMonsterGun>()->setType(Monster_type::shotgun);

	CGameObject* pMonsterArm = new CGameObject;
	pMonsterArm->SetName(L"MonsterArm");
	pMonsterArm->AddComponent(new CTransform);
	pMonsterArm->AddComponent(new CMeshRender);
	pMonsterArm->AddComponent(new CMonsterArm);

	pMonsterArm->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pMonsterArm->Transform()->SetRelativeScale(Vec3(12.f, 12.f, 0.f));
	pMonsterArm->Transform()->SetIgnoreParentScale(true);

	pMonsterArm->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterArm->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MonsterArmMtrl"));
	pMonsterArm->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\bullet_hand_001.png"));

	pMonsterArm->GetScript<CMonsterArm>()->setOffset(Vec3(0.f, -20.f, 0.f));
	pMonsterArm->GetScript<CMonsterArm>()->setArmpos(Vec3(30.f, 0.f, 0.f));
	pMonsterArm->GetScript<CMonsterArm>()->setType(Monster_type::shotgun);
	pMonsterArm->AddChild(pMonsterGun);
	pMonster->AddChild(pMonsterArm);
	Monprefab = new CPrefab(pMonster, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"ShotgunKinRed", Monprefab.Get());

	pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(LoadAnimator2D(L"animation\\shotgunkin_blue.anim"));
	pMonster->AddComponent(new CRigidBody);
	pMonster->AddComponent(new CShotgunBulletKinScript);

	pMonster->Transform()->SetRelativePos(Vec3(200.f, 800.f, 75.f));
	pMonster->Transform()->SetRelativeScale(Vec3(140.f, 120.f, 0.f));

	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ShotgunkinredMtrl"));


	pMonster->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pMonster->Collider2D()->SetIgnoreObjectScale(true);
	pMonster->Collider2D()->SetScale(Vec2(40.f, 55.f));
	pMonster->Collider2D()->SetOffsetPos(Vec2(0.f, -15.f));

	pMonsterGun = new CGameObject;
	pMonsterGun->SetName(L"MonsterGun");

	pMonsterGun->AddComponent(new CTransform);
	pMonsterGun->AddComponent(new CMeshRender);
	pMonsterGun->AddComponent(new CMonsterGun);

	pMonsterGun->Transform()->SetRelativePos(Vec3(20.f, 0.f, 0.f));
	pMonsterGun->Transform()->SetRelativeScale(Vec3(60.f, 15.f, 0.f));
	pMonsterGun->Transform()->SetIgnoreParentScale(true);

	pMonsterGun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterGun->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MonsterShotgunMtrl"));
	pMonsterGun->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\shotgun_idle_001.png"));
	pMonsterGun->GetScript<CMonsterGun>()->setType(Monster_type::shotgun);

	pMonsterArm = new CGameObject;
	pMonsterArm->SetName(L"MonsterArm");
	pMonsterArm->AddComponent(new CTransform);
	pMonsterArm->AddComponent(new CMeshRender);
	pMonsterArm->AddComponent(new CMonsterArm);

	pMonsterArm->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pMonsterArm->Transform()->SetRelativeScale(Vec3(12.f, 12.f, 0.f));
	pMonsterArm->Transform()->SetIgnoreParentScale(true);

	pMonsterArm->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterArm->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MonsterArmMtrl"));
	pMonsterArm->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\bullet_hand_001.png"));

	pMonsterArm->GetScript<CMonsterArm>()->setOffset(Vec3(0.f, -20.f, 0.f));
	pMonsterArm->GetScript<CMonsterArm>()->setArmpos(Vec3(30.f, 0.f, 0.f));
	pMonsterArm->GetScript<CMonsterArm>()->setType(Monster_type::shotgun);
	pMonsterArm->AddChild(pMonsterGun);
	pMonster->AddChild(pMonsterArm);
	Monprefab = new CPrefab(pMonster, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"ShotgunKinBlue", Monprefab.Get());

	pMonster = new CGameObject;
	pMonster->SetName(L"Monster");
	
	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(LoadAnimator2D(L"animation\\bulletking.anim"));
	pMonster->AddComponent(new CBulletKingScript);

	pMonster->Transform()->SetRelativeScale(Vec3(260.f, 280.f, 0.f));
	
	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"bulletkingMtrl"));
	
	pMonster->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pMonster->Collider2D()->SetIgnoreObjectScale(true);
	pMonster->Collider2D()->SetScale(Vec2(130.f, 200.f));
	pMonster->Collider2D()->SetOffsetPos(Vec2(0.f, -20.f));
	
	Monprefab = new CPrefab(pMonster, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"BulletKing", Monprefab.Get());

	pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(LoadAnimator2D(L"animation\\bulletkin.anim"));
	pMonster->AddComponent(new CRigidBody);
	pMonster->AddComponent(new CBulletKinScript);

	pMonster->Transform()->SetRelativePos(Vec3(0.f, 800.f, 75.f));
	pMonster->Transform()->SetRelativeScale(Vec3(100.f, 120.f, 0.f));

	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"bulletkinMtrl"));

	
	pMonster->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pMonster->Collider2D()->SetIgnoreObjectScale(true);
	pMonster->Collider2D()->SetScale(Vec2(35.f, 55.f));
	pMonster->Collider2D()->SetOffsetPos(Vec2(0.f, -5.f));

	pMonsterGun = new CGameObject;
	pMonsterGun->SetName(L"MonsterGun");

	pMonsterGun->AddComponent(new CTransform);
	pMonsterGun->AddComponent(new CMeshRender);
	pMonsterGun->AddComponent(new CMonsterGun);

	pMonsterGun->Transform()->SetRelativePos(Vec3(20.f, 0.f, 0.f));
	pMonsterGun->Transform()->SetRelativeScale(Vec3(50.f, 45.f, 0.f));
	pMonsterGun->Transform()->SetIgnoreParentScale(true);

	pMonsterGun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterGun->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MonsterMagnumMtrl"));
	pMonsterGun->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\magnum_idle_001.png"));
	pMonsterGun->GetScript<CMonsterGun>()->setType(Monster_type::magnum);

	pMonsterArm = new CGameObject;
	pMonsterArm->SetName(L"MonsterArm");
	pMonsterArm->AddComponent(new CTransform);
	pMonsterArm->AddComponent(new CMeshRender);
	pMonsterArm->AddComponent(new CMonsterArm);

	pMonsterArm->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pMonsterArm->Transform()->SetRelativeScale(Vec3(12.f, 12.f, 0.f));
	pMonsterArm->Transform()->SetIgnoreParentScale(true);

	pMonsterArm->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonsterArm->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MonsterArmMtrl"));
	pMonsterArm->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\bullet_hand_001.png"));

	pMonsterArm->GetScript<CMonsterArm>()->setOffset(Vec3(0.f, -20.f, 0.f));
	pMonsterArm->GetScript<CMonsterArm>()->setArmpos(Vec3(30.f, 0.f, 0.f));
	pMonsterArm->GetScript<CMonsterArm>()->setType(Monster_type::magnum);
	pMonsterArm->AddChild(pMonsterGun);
	pMonster->AddChild(pMonsterArm);
	Monprefab = new CPrefab(pMonster, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"BulletKin", Monprefab.Get());

	CResMgr::GetInst()->FindRes<CMaterial>(L"eggBulletMtrl")->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\football_egg_projectile_001.png"));

	// 충돌 레이어 설정
	CCollisionMgr::GetInst()->CollisionLayerCheck(1, 3);
	CCollisionMgr::GetInst()->CollisionLayerCheck(1, 4);
	CCollisionMgr::GetInst()->CollisionLayerCheck(2, 3);
	CCollisionMgr::GetInst()->CollisionLayerCheck(1, 5);
	CCollisionMgr::GetInst()->CollisionLayerCheck(2, 5);
	CCollisionMgr::GetInst()->CollisionLayerCheck(3, 5);
	CCollisionMgr::GetInst()->CollisionLayerCheck(4, 5);
	CCollisionMgr::GetInst()->CollisionLayerCheck(3, 3);
	CCollisionMgr::GetInst()->CollisionLayerCheck(1, 6);
	CCollisionMgr::GetInst()->CollisionLayerCheck(1, 7);
	CCollisionMgr::GetInst()->CollisionLayerCheck(2, 7);
	CCollisionMgr::GetInst()->CollisionLayerCheck(3, 7);
	CCollisionMgr::GetInst()->CollisionLayerCheck(4, 7);
	CCollisionMgr::GetInst()->CollisionLayerCheck(5, 7);
	CCollisionMgr::GetInst()->CollisionLayerCheck(7, 7);
	CCollisionMgr::GetInst()->CollisionLayerCheck(1, 8);
	CCollisionMgr::GetInst()->CollisionLayerCheck(2, 8);
	CCollisionMgr::GetInst()->CollisionLayerCheck(4, 9);
	// UI
	CGameObject* pUI = new CGameObject;
	pUI->SetName(L"heart_full");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(-687.f, 373.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(64.f, 64.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"heart_fullMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\heart_full.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"heart_full", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"heart_half");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(-639.f, 373.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(64.f, 64.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"heart_halfMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\heart_half.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"heart_half", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"heart_empty");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(-639.f, 373.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(64.f, 64.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"heart_emptyMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\heart_empty.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"heart_empty", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"BossHPBar");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativeScale(Vec3(600.f, 42.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BossHPBar_UIMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\BossHPBar.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"BossHPBar", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"BossHP");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativeScale(Vec3(498.f, 42.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BossHP_UIMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\BossHP.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"BossHP", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"blank");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(-692.f, 334.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(36.f, 36.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"blankMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\blank.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"blank", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"ItemBox");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(615.f, -350.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(135.f, 87.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"item_boxMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\item_box.png"));
	pLevel->AddGameObject(pUI, 31);

	pUI = new CGameObject;
	pUI->SetName(L"bulletUI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(700.f, -370.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(16.f, 10.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"bulletMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\bullet.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"bulletUI", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"bullet_emptyUI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(700.f, -354.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(16.f, 10.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"bullet_emptyMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\bullet_empty.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"bullet_emptyUI", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"bullet_topUI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(700.f, -338.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(16.f, 10.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"bullet_topMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\bullet_top.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"bullet_topUI", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"bullet_bottomUI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(700.f, -386.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(16.f, 10.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"bullet_bottomMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\bullet_bottom.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"bullet_bottomUI", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"coinUI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativePos(Vec3(-692.f, 290.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(36.f, 36.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"coinMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\coin.png"));
	pLevel->AddGameObject(pUI, 31);
	/*prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"coinUI", prefab.Get());*/
	
	pUI = new CGameObject;
	pUI->SetName(L"ReloadBarUI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativeScale(Vec3(108.f, 15.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ReloadBar_UIMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\reload_bar.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"ReloadBarUI", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"ReloadUI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);

	pUI->Transform()->SetRelativeScale(Vec3(3.f, 15.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Reload_UIMtrl"));
	pUI->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\reload_v.png"));
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"ReloadUI", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"rusty_sidearm_UI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);
	pUI->AddComponent(LoadAnimator2D(L"animation\\rust_sidearm.anim"));

	pUI->Transform()->SetRelativePos(Vec3(615.f, -350.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(50.f, 47.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rusty_sidearm_UIMtrl"));
	pUI->Animator2D()->Play(L"IDLE", true);
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"rusty_sidearm_UI", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"shotgun_UI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);
	pUI->AddComponent(LoadAnimator2D(L"animation\\shotgun.anim"));

	pUI->Transform()->SetRelativePos(Vec3(615.f, -350.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(60.f, 40.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rusty_sidearm_UIMtrl"));
	pUI->Animator2D()->Play(L"IDLE", true);
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"shotgun_UI", prefab.Get());

	pUI = new CGameObject;
	pUI->SetName(L"megahand_UI");
	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);
	pUI->AddComponent(LoadAnimator2D(L"animation\\megahand.anim"));

	pUI->Transform()->SetRelativePos(Vec3(615.f, -350.f, 1.f));
	pUI->Transform()->SetRelativeScale(Vec3(44.f, 40.f, 0.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rusty_sidearm_UIMtrl"));
	pUI->Animator2D()->Play(L"IDLE", true);
	prefab = new CPrefab(pUI, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"megahand_UI", prefab.Get());

	Vec3 GUNUIPos = Vec3(615.f, -350.f, 1.f);
	CGameObject* UIGUN = CResMgr::GetInst()->FindRes<CPrefab>(L"rusty_sidearm_UI")->Instantiate();
	Instantiate(UIGUN, GUNUIPos, 31, true, pLevel);

	pUI = new CGameObject;
	pUI->SetName(L"Game_UI");
	pUI->AddComponent(new CUIScript);
	pUI->GetScript<CUIScript>()->setCurrentGun(UIGUN);
	pLevel->AddGameObject(pUI, 31);

	pObject->GetScript<CPlayerScript>()->setUI(pUI);
	pGun->GetScript<CGunScript>()->setUI(pUI);
	pUI->GetScript<CUIScript>()->setPlayer(pObject);

	{
		pBullet = new CGameObject;
		pBullet->SetName(L"Bullet");
		pBullet->AddComponent(new CTransform);
		pBullet->AddComponent(new CMeshRender);
		pBullet->AddComponent(new CCollider2D);
		pBullet->AddComponent(new CBulletScript);

		pBullet->Transform()->SetRelativePos(Vec3(0.f, 0.f, 75.f));
		pBullet->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));

		pBullet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pBullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmBulletMtrl"));
		pBullet->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\player_Projectile.png"));
		pBullet->GetScript<CBulletScript>()->setGUN(GUN::shotgun);
		pBullet->GetScript<CBulletScript>()->setDuration(0.5f);
		pBullet->GetScript<CBulletScript>()->setDMG(4);
		prefab = new CPrefab(pBullet, true);
		CResMgr::GetInst()->AddRes<CPrefab>(L"shotgunBullet", prefab.Get());

		pGun = new CGameObject;
		pGun->SetName(L"Gun");

		pGun->AddComponent(new CTransform);
		pGun->AddComponent(new CMeshRender);
		pGun->AddComponent(LoadAnimator2D(L"animation\\shotgun.anim"));
		pGun->AddComponent(new CGunScript);

		pGun->Transform()->SetRelativePos(Vec3(20.f, 5.f, 0.f));
		pGun->Transform()->SetRelativeScale(Vec3(60.f, 40.f, 0.f));
		pGun->Transform()->SetIgnoreParentScale(true);

		pGun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pGun->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmMtrl"));
		pGun->GetScript<CGunScript>()->setGUN(GUN::shotgun);
		pGun->GetScript<CGunScript>()->setReloadtime(1.2f);
		pGun->GetScript<CGunScript>()->setBulletSpread(20.f);
		pGun->GetScript<CGunScript>()->setFireInterval(0.5f);
		pGun->GetScript<CGunScript>()->setMaximumBullet(250);
		pGun->Animator2D()->Play(L"IDLE", true);
		pGun->GetScript<CGunScript>()->setBullet(prefab);
		pGun->GetScript<CGunScript>()->setUI(pUI);
		prefab = new CPrefab(pGun, true);
		CResMgr::GetInst()->AddRes<CPrefab>(L"shotgun", prefab.Get());

		pBullet = new CGameObject;
		pBullet->SetName(L"Bullet");
		pBullet->AddComponent(new CTransform);
		pBullet->AddComponent(new CMeshRender);
		pBullet->AddComponent(new CCollider2D);
		pBullet->AddComponent(LoadAnimator2D(L"animation\\bullet_megahand.anim"));
		pBullet->AddComponent(new CBulletScript);

		pBullet->Transform()->SetRelativePos(Vec3(0.f, 0.f, 75.f));
		pBullet->Transform()->SetRelativeScale(Vec3(18.f, 14.f, 0.f));

		pBullet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pBullet->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmBulletMtrl"));
		pBullet->GetScript<CBulletScript>()->setGUN(GUN::megahand);
		pBullet->GetScript<CBulletScript>()->setDMG(6);
		pBullet->GetScript<CBulletScript>()->setSpeed(900.f);
		prefab = new CPrefab(pBullet, true);
		CResMgr::GetInst()->AddRes<CPrefab>(L"megahandBullet", prefab.Get());

		pGun = new CGameObject;
		pGun->SetName(L"Gun");

		pGun->AddComponent(new CTransform);
		pGun->AddComponent(new CMeshRender);
		pGun->AddComponent(LoadAnimator2D(L"animation\\megahand.anim"));
		pGun->AddComponent(new CGunScript);

		pGun->Transform()->SetRelativePos(Vec3(10.f, 0.f, 0.f));
		pGun->Transform()->SetRelativeScale(Vec3(44.f, 40.f, 0.f));
		pGun->Transform()->SetIgnoreParentScale(true);

		pGun->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pGun->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"rust_sidearmMtrl"));
		pGun->GetScript<CGunScript>()->setGUN(GUN::megahand);
		pGun->GetScript<CGunScript>()->setMagazineBullet(18);
		pGun->GetScript<CGunScript>()->setReloadtime(0.54f);
		pGun->GetScript<CGunScript>()->setFireInterval(0.07f);
		pGun->GetScript<CGunScript>()->setMaximumBullet(200);
		pGun->Animator2D()->Play(L"IDLE", true);
		pGun->GetScript<CGunScript>()->setBullet(prefab);
		pGun->GetScript<CGunScript>()->setUI(pUI);
		prefab = new CPrefab(pGun, true);
		CResMgr::GetInst()->AddRes<CPrefab>(L"megahand", prefab.Get());
	}

	// Effect
	CGameObject* pEffect = new CGameObject;
	pEffect->SetName(L"bullet_impact");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\bullet_impact.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(32.f, 32.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(0.3125f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"bullet_impact", prefab.Get());

	pEffect = new CGameObject;
	pEffect->SetName(L"dustup");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\dustup.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(64.f, 64.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(0.3125f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"dustup", prefab.Get());

	pEffect = new CGameObject;
	pEffect->SetName(L"dustup_run");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\dustup_run.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(24.f, 24.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(0.5f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"dustup_run", prefab.Get());

	pEffect = new CGameObject;
	pEffect->SetName(L"dustup_dodge");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\dustup_dodge.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(64.f, 32.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(0.5f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"dustup_dodge", prefab.Get());

	pEffect = new CGameObject;
	pEffect->SetName(L"dustup_wall");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\dustup_wall.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(48.f, 24.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(0.4375f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"dustup_wall", prefab.Get());

	pEffect = new CGameObject;
	pEffect->SetName(L"teleport_dustup");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\teleport_dustup.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(162.f, 154.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(0.5f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"teleport_dustup", prefab.Get());

	pEffect = new CGameObject;
	pEffect->SetName(L"explosion_big");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\explosion_big.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(197.f, 190.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(1.f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"explosion_big", prefab.Get());

	pEffect = new CGameObject;
	pEffect->SetName(L"megabuster_impact");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\megabuster_impact.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(197.f, 190.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(0.5f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"megabuster_impact", prefab.Get());

	pEffect = new CGameObject;
	pEffect->SetName(L"monsterspawn");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\monsterspawn.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(360.f, 291.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(1.375f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"monsterspawn", prefab.Get());

	pEffect = new CGameObject;
	pEffect->SetName(L"blankeffect");
	pEffect->AddComponent(new CTransform);
	pEffect->AddComponent(new CMeshRender);
	pEffect->AddComponent(LoadAnimator2D(L"animation\\blankeffect.anim"));
	pEffect->AddComponent(new CEffectScript);
	pEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pEffect->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EffectMtrl"));
	pEffect->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 1.f));

	pEffect->GetScript<CEffectScript>()->setEffecttime(0.5f);
	prefab = new CPrefab(pEffect, true);
	CResMgr::GetInst()->AddRes<CPrefab>(L"blankeffect", prefab.Get());

	// 레벨 설정
	CLevelMgr::GetInst()->setLevel(pLevel);	
}

void CreateTitleLevel()
{
	CLevel* pLevel = new CLevel;

	// Layer 이름 설정
	pLevel->GetLayer(1)->SetName(L"Player");
	pLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(3)->SetName(L"Monster");
	pLevel->GetLayer(4)->SetName(L"MonsterProjectile");
	pLevel->GetLayer(5)->SetName(L"Map");
	pLevel->GetLayer(6)->SetName(L"Item");
	pLevel->GetLayer(7)->SetName(L"Object");
	pLevel->GetLayer(31)->SetName(L"UI");


	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Camera()->SetLayerMaskAll();
	pCamObj->Camera()->SetLayerMask(31);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);

	pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	pLevel->AddGameObject(pCamObj, 0);

	// Directional Light 추가
	CGameObject* pDirLight = new CGameObject;
	pDirLight->SetName(L"DirectionalLight");

	pDirLight->AddComponent(new CTransform);
	pDirLight->AddComponent(new CLight2D);

	pDirLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pDirLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

	pLevel->AddGameObject(pDirLight, 0);

	// title
	CGameObject* CTitle = new CGameObject;
	CTitle->SetName(L"Title");
	CTitle->AddComponent(new CTransform);
	CTitle->AddComponent(new CMeshRender);

	CTitle->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	CTitle->Transform()->SetRelativeScale(Vec3(1439.f, 810.f, 0.f));

	CTitle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CTitle->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"titleMtrl"));

	pLevel->AddGameObject(CTitle, 0);

	CTitle = new CGameObject;
	CTitle->SetName(L"play");
	CTitle->AddComponent(new CTransform);
	CTitle->AddComponent(new CMeshRender);
	CTitle->AddComponent(new CButtonScript);
	
	CTitle->Transform()->SetRelativePos(Vec3(-620.f, -260.f, 50.f));
	CTitle->Transform()->SetRelativeScale(Vec3(78.f, 32.f, 0.f));

	CTitle->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	CTitle->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"playMtrl"));

	pLevel->AddGameObject(CTitle, 0);
	

	CGameObject* pLich = new CGameObject;
	pLich->SetName(L"lich_top");
	pLich->AddComponent(new CTransform);
	pLich->AddComponent(new CMeshRender);
	pLich->AddComponent(LoadAnimator2D(L"animation\\lich_top.anim"));
	pLich->AddComponent(new CENVScript(ENV_Type::lich));

	pLich->Transform()->SetRelativePos(Vec3(-570.f, 180.f, 68.f));
	pLich->Transform()->SetRelativeScale(Vec3(297.f, 437.f, 0.f));

	pLich->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pLich->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pLevel->AddGameObject(pLich, 0);

	pLich = new CGameObject;
	pLich->SetName(L"lich_bottom");
	pLich->AddComponent(new CTransform);
	pLich->AddComponent(new CMeshRender);
	pLich->AddComponent(LoadAnimator2D(L"animation\\lich_bottom.anim"));
	pLich->AddComponent(new CENVScript);

	pLich->Transform()->SetRelativePos(Vec3(-320.f, -130.f, 70.f));
	pLich->Transform()->SetRelativeScale(Vec3(800.f, 471.f, 0.f));

	pLich->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pLich->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pLevel->AddGameObject(pLich, 0);

	pLich = new CGameObject;
	pLich->SetName(L"lich_hand");
	pLich->AddComponent(new CTransform);
	pLich->AddComponent(new CMeshRender);
	pLich->AddComponent(LoadAnimator2D(L"animation\\lich_hand.anim"));
	pLich->AddComponent(new CENVScript);

	pLich->Transform()->SetRelativePos(Vec3(-478.f, 205.f, 65.f));
	pLich->Transform()->SetRelativeScale(Vec3(185.f, 482.f, 0.f));

	pLich->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pLich->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pLevel->AddGameObject(pLich, 0);
	CLevelMgr::GetInst()->ChangeLevel(pLevel);
}

CGameObject* LoadTilemap(wstring _strRelativePath)
{
	// 경로
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// 파일 읽기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	CGameObject* tilemap = LoadGameObject(pFile);

	fclose(pFile);



	return tilemap;
}

CGameObject* LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// 오브젝트 이름
	pObject->LoadFromFile(_File);

	// 오브젝트 소유 컴포넌트
	bool bProgress = true;
	while (true)
	{
		// Component 타입 확인
		COMPONENT_TYPE type = COMPONENT_TYPE::END;
		fread(&type, sizeof(UINT), 1, _File);

		// 타입에 해당하는 컴포넌트 로딩 및 추가
		CComponent* pComponent = nullptr;

		switch (type)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new CTransform;
			break;

		case COMPONENT_TYPE::CAMERA:
			pComponent = new CCamera;
			break;

		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new CCollider2D;
			break;

		case COMPONENT_TYPE::ANIMATOR2D:
			pComponent = new CAnimator2D;
			break;

		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new CLight2D;
			break;

		case COMPONENT_TYPE::RIGIDBODY:
			pComponent = new CRigidBody;
			break;

		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new CMeshRender;
			break;

		case COMPONENT_TYPE::TILEMAP:
			pComponent = new CTileMap;
			break;

		case COMPONENT_TYPE::PARTICLESYSTEM:
			pComponent = new CParticleSystem;
			break;

		case COMPONENT_TYPE::LIGHT3D:
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			break;
		case COMPONENT_TYPE::SKYBOX:
			break;
		case COMPONENT_TYPE::DECAL:
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			break;
		case COMPONENT_TYPE::END:
			bProgress = false;
			break;
		}

		if (!bProgress)
			break;

		pComponent->LoadFromFile(_File);
		pObject->AddComponent(pComponent);
	}


	// Script
	size_t iScriptCount = 0;
	fread(&iScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iScriptCount; ++i)
	{
		wstring strScriptName;
		LoadWStringFromFile(strScriptName, _File);
		CScript* pNewScript = CScriptMgr::GetScript(strScriptName);
		pNewScript->LoadFromFile(_File);

		pObject->AddComponent(pNewScript);
	}


	// 자식 오브젝트
	size_t iChildCount = 0;
	fread(&iChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iChildCount; ++i)
	{
		CGameObject* pChild = LoadGameObject(_File);
		pObject->AddChild(pChild);
	}

	return pObject;
}

CComponent* LoadAnimator2D(wstring _strRelativePath)
{
	// 경로
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// 파일 읽기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	COMPONENT_TYPE type = COMPONENT_TYPE::END;
	fread(&type, sizeof(UINT), 1, pFile);

	CComponent* pComponent = nullptr;

	if (type == COMPONENT_TYPE::ANIMATOR2D)
	{
		pComponent = new CAnimator2D;
		pComponent->LoadFromFile(pFile);
	}

	fclose(pFile);

	return pComponent;
}


