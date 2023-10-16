#include "pch.h"
#include "CResMgr.h"
#include "CEventMgr.h"

#include "CSound.h"
#include "CGameObject.h"
#include "GlobalComponent.h"

void CResMgr::init()
{
	InitSound();

	LoadContent();

	CreateDefaultMesh();

	CreateDefaultTexture();

	CreateDefaultGraphicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();
}

void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;
	CMesh* pMesh = nullptr;

	// Point Mesh
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT idx = 0;

	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &idx, 1);
	AddRes<CMesh>(L"PointMesh", pMesh);	


	// 사각형 메쉬 만들기
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	// Index
	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);	
	vecIdx.clear();





	// Index
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);	

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();


	// 원형메쉬 만들기
	// 중심점	
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);

	vecVtx.push_back(v);

	int iSlice = 40;
	float fRadius = 0.5f;
	float fTheta = XM_2PI / (float)iSlice;

	for (int i = 0; i < iSlice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));

		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < (UINT)iSlice; ++i)
	{
		vecIdx.push_back(0);
		if (i == iSlice - 1)
		{
			vecIdx.push_back(1);
		}
		else
		{
			vecIdx.push_back(i + 2);
		}
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh);	
	vecIdx.clear();


	// Cicle_debug
	for (int i = 0; i < vecVtx.size() - 2; ++i)
	{		
		vecIdx.push_back(i + 1);				
	}
	vecIdx.push_back(1);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void CResMgr::CreateDefaultTexture()
{
	// NoiseTexture
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg");

	// 텍스쳐 로딩
	Load<CTexture>(L"texture\\tile\\tileset_castle.png", L"texture\\tile\\tileset_castle.png");
	Load<CTexture>(L"texture\\tile\\throne_room_dais_001.png", L"texture\\tile\\throne_room_dais_001.png");
	Load<CTexture>(L"texture\\tile\\throne_room_wall_001.png", L"texture\\tile\\throne_room_wall_001.png");
	Load<CTexture>(L"texture\\tile\\throne_room_wall_hang_001.png", L"texture\\tile\\throne_room_wall_hang_001.png");
	Load<CTexture>(L"texture\\tile\\Boss_Door_east_mat_001.png", L"texture\\tile\\Boss_Door_east_mat_001.png");
	Load<CTexture>(L"texture\\tile\\counter.png", L"texture\\tile\\counter.png");
	Load<CTexture>(L"texture\\table.png", L"texture\\table.png");
	Load<CTexture>(L"texture\\guide_hand.png", L"texture\\guide_hand.png");
	Load<CTexture>(L"texture\\bullet_hand_001.png", L"texture\\bullet_hand_001.png");
	Load<CTexture>(L"texture\\gun\\shotgun_idle_001.png", L"texture\\gun\\shotgun_idle_001.png");
	Load<CTexture>(L"texture\\gun\\magnum_idle_001.png", L"texture\\gun\\magnum_idle_001.png");
	Load<CTexture>(L"texture\\gun\\mega_buster_idle_001.png", L"texture\\gun\\mega_buster_idle_001.png");
	Load<CTexture>(L"texture\\gun\\scope_001.png", L"texture\\gun\\scope_001.png");
	Load<CTexture>(L"texture\\gun\\player_Projectile.png", L"texture\\gun\\player_Projectile.png");
	Load<CTexture>(L"texture\\enemy_projectile.png", L"texture\\enemy_projectile.png");
	Load<CTexture>(L"texture\\football_egg_projectile_001.png", L"texture\\football_egg_projectile_001.png");
	Load<CTexture>(L"texture\\castle_door_block_horizontal_up_001.png", L"texture\\castle_door_block_horizontal_up_001.png");
	Load<CTexture>(L"texture\\castle_door_block_vertical_headshake_001.png", L"texture\\castle_door_block_vertical_headshake_001.png");
	Load<CTexture>(L"texture\\UI\\shop_heart.png", L"texture\\UI\\shop_heart.png");
	Load<CTexture>(L"texture\\UI\\shop_blank.png", L"texture\\UI\\shop_blank.png");
	Load<CTexture>(L"texture\\UI\\shop_megahand.png", L"texture\\UI\\shop_megahand.png");
	Load<CTexture>(L"texture\\UI\\shop_scope.png", L"texture\\UI\\shop_scope.png");
	Load<CTexture>(L"texture\\sold_out_sign_001.png", L"texture\\sold_out_sign_001.png");
	Load<CTexture>(L"texture\\title\\play.png", L"texture\\title\\play.png");
	Load<CTexture>(L"texture\\title\\title.png", L"texture\\title\\title.png");

	// 텍스쳐 생성
	CreateTexture(L"UAVTex", 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
}

void CResMgr::CreateDefaultGraphicsShader()
{
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "COLOR");
	AddInputLayout(DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD");

	CGraphicsShader* pShader = nullptr;

	// Std2D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam(INT_0,	 "Test Int");
	pShader->AddScalarParam(FLOAT_2, "Test Float");
	pShader->AddScalarParam(VEC2_3, "Test Vec2");
	pShader->AddScalarParam(VEC4_2, "Test Vec4");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGraphicsShader>(L"Std2DShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_nolight");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam(INT_0, "Test Int");
	pShader->AddScalarParam(FLOAT_2, "Test Float");
	pShader->AddScalarParam(VEC2_3, "Test Vec2");
	pShader->AddScalarParam(VEC4_2, "Test Vec4");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGraphicsShader>(L"Std2DShader_nolight", pShader);

	// Std2dAlphaBlend Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddTexureParam(TEX_0, "Output Texture");
	

	AddRes<CGraphicsShader>(L"Std2DAlphaBlendShader", pShader);


	// Grid Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\editor.fx", "VS_Grid");
	pShader->CreatePixelShader(L"shader\\editor.fx", "PS_Grid");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"GridShader", pShader);

	// DebugDraw Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\debugdraw.fx", "VS_DebugDraw");
	pShader->CreatePixelShader(L"shader\\debugdraw.fx", "PS_DebugDraw");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"DebugDrawShader", pShader);


	// ParticleRenderShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\particlerender.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particlerender.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particlerender.fx", "PS_ParticleRender");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"ParticleRenderShader", pShader);


	// PostProcess Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_PostProcess");	
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_PostProcess");
	
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);

	AddRes<CGraphicsShader>(L"PostProcessShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_PostProcess");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_PostProcess_Darkroom");

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);

	AddRes<CGraphicsShader>(L"PostProcessDarkroomShader", pShader);

	// TileMap Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");
		
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	AddRes<CGraphicsShader>(L"TileMapShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_nolight");
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes<CGraphicsShader>(L"UIShader", pShader);
}


#include "CPaintShader.h"
#include "CParticleUpdateShader.h"
void CResMgr::CreateDefaultComputeShader()
{
	CComputeShader* pShader = nullptr;

	// Paint Shader
	pShader = new CPaintShader;
	pShader->CreateComputeShader(L"shader\\compute.fx", "CS_Paint");
	AddRes<CComputeShader>(L"PaintShader", pShader);

	// Particle Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pShader);

	//pShader = new CParticleUpdateShader;
	//pShader->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	//AddRes<CComputeShader>(L"ParticleUpdateShader", pShader);
	//
	//pShader = new CParticleUpdateShader;
	//pShader->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	//AddRes<CComputeShader>(L"ParticleUpdateShader", pShader);
}

void CResMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = nullptr;

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));	
	AddRes<CMaterial>(L"Std2DMtrl", pMtrl);

	//Load<CMaterial>(L"Std2DMtrl", L"material\\mtrl.mtrl");

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DAlphaBlendShader"));
	AddRes<CMaterial>(L"Std2DAlphaBlendMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"GridShader"));
	AddRes<CMaterial>(L"GridMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugDrawShader"));
	AddRes<CMaterial>(L"DebugDrawMtrl", pMtrl);	

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DeadCellShader"));
	AddRes<CMaterial>(L"DeadCellMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes<CMaterial>(L"ParticleRenderMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"EffectMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PostProcessShader"));
	AddRes<CMaterial>(L"PostProcessMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PostProcessDarkroomShader"));
	AddRes<CMaterial>(L"DarkroomMtrl", pMtrl);
	
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes<CMaterial>(L"TileMapMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"PlayerArmMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"rust_sidearmMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader_nolight"));
	AddRes<CMaterial>(L"rust_sidearmBulletMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"BooklletMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"WizardMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"GunnutMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"ShotgunkinredMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"ShotgunkinblueMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"bulletkingMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"bulletkinMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader_nolight"));
	AddRes<CMaterial>(L"MonsterBulletMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"MonsterShotgunMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"MonsterMagnumMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"MonsterArmMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader_nolight"));
	AddRes<CMaterial>(L"eggBulletMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"ItemShotgunMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"ItemMegahandMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"ItemWaxwingMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"ItemScopeMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\sold_out_sign_001.png"));
	AddRes<CMaterial>(L"ItemSoldOutMtrl", pMtrl);


	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"wall_door_vMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"wall_door_hMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\shop_heart.png"));
	AddRes<CMaterial>(L"shop_heartMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\shop_megahand.png"));
	AddRes<CMaterial>(L"shop_megahandMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\shop_scope.png"));
	AddRes<CMaterial>(L"shop_scopeMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\shop_blank.png"));
	AddRes<CMaterial>(L"shop_blankMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\blank.png", L"texture\\UI\\blank.png"));
	AddRes<CMaterial>(L"itemblankMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\UI\\heart_full.png", L"texture\\UI\\heart_full.png"));
	AddRes<CMaterial>(L"itemheartMtrl", pMtrl);
	//UI
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"heart_fullMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"heart_halfMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"heart_emptyMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"bullet_topMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"bullet_bottomMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"bullet_emptyMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"bulletMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"blankMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"coinMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"item_boxMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"keyMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"rusty_sidearm_UIMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"BossHPBar_UIMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes<CMaterial>(L"BossHP_UIMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader_nolight"));
	AddRes<CMaterial>(L"ReloadBar_UIMtrl", pMtrl);
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader_nolight"));
	AddRes<CMaterial>(L"Reload_UIMtrl", pMtrl);

	//Image
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\throne_room_dais_001.png"));
	AddRes<CMaterial>(L"BossImage1Mtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\throne_room_wall_hang_001.png"));
	AddRes<CMaterial>(L"BossImage2Mtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\Boss_Door_east_mat_001.png"));
	AddRes<CMaterial>(L"BossImage0Mtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\counter.png"));
	AddRes<CMaterial>(L"shopImage1Mtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\title.png"));
	AddRes<CMaterial>(L"titleMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	pMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\play.png"));
	AddRes<CMaterial>(L"playMtrl", pMtrl);
}

void CResMgr::LoadContent()
{
	m_vecContentName.clear();

	// Content 폴더에 있는 모든 리소스들을 검사 및 로딩
	wstring strFolderPath = CPathMgr::GetInst()->GetContentPath();
	FindContentFileName(strFolderPath);

	// 파일명의 확장자를 확인 후 알맞은 리소스가 무엇인지 알아낸다.
	for (size_t i = 0; i < m_vecContentName.size(); ++i)
	{
		RES_TYPE resType = GetResTypeByExt(m_vecContentName[i]);

		// 리소스 타입에 해당하는 리소스를 경로로부터 로딩
		if (RES_TYPE::END == resType)
			continue;

		switch (resType)
		{
		case RES_TYPE::PREFAB:
			CResMgr::GetInst()->Load<CPrefab>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::MESHDATA:

			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::MESH:
			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(m_vecContentName[i], m_vecContentName[i]);
			break;
		case RES_TYPE::SOUND:
			CResMgr::GetInst()->Load<CSound>(m_vecContentName[i], m_vecContentName[i]);
			break;
		}
	}
}

void CResMgr::FindContentFileName(const wstring& _strFolderPath)
{
	// 모든 파일명을 알아낸다.
	wstring strFolderPath = _strFolderPath + L"*.*";

	HANDLE hFindHandle = nullptr;

	WIN32_FIND_DATA data = {};

	// 탐색용 핸들 얻기
	hFindHandle = FindFirstFile(strFolderPath.c_str(), &data);

	if (INVALID_HANDLE_VALUE == hFindHandle)
		return;

	// 탐색 핸들을 이용해서 모든 파일을 다 확인할 때 까지 반복
	while (FindNextFile(hFindHandle, &data))
	{
		if ((FILE_ATTRIBUTE_DIRECTORY & data.dwFileAttributes) && wcscmp(data.cFileName, L".."))
		{
			FindContentFileName(_strFolderPath + data.cFileName + L"\\");
		}

		else
		{
			wstring strRelative = GetRelativePath(CPathMgr::GetInst()->GetContentPath(), _strFolderPath + data.cFileName);
			m_vecContentName.push_back(strRelative);
		}
	}

	FindClose(hFindHandle);
}

RES_TYPE CResMgr::GetResTypeByExt(wstring _filename)
{
	wstring strExt = path(_filename.c_str()).extension();

	if (strExt == L".pref")
		return RES_TYPE::PREFAB;
	else if (strExt == L".mdat")
		return RES_TYPE::MESHDATA;
	else if (strExt == L".mtrl")
		return RES_TYPE::MATERIAL;
	else if (strExt == L".mesh")
		return RES_TYPE::MESH;
	else if (strExt == L".dds" || strExt == L".DDS" || strExt == L".tag" || strExt == L".TGA"
		|| strExt == L".png" || strExt == L".jpg" || strExt == L".JPG" || strExt == L".jegp" || strExt == L".JPEG"
		|| strExt == L".bmp" || strExt == L".BMP")
		return RES_TYPE::TEXTURE;
	else if (strExt == L".mp3" || strExt == L".MP3" || strExt == L".wav" || strExt == L".WAV" || strExt == L".ogg" || strExt == L".OGG")
		return RES_TYPE::SOUND;
	else
		return RES_TYPE::END;
}


void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName)
{
	D3D11_INPUT_ELEMENT_DESC LayoutDesc = {};
	LayoutDesc.AlignedByteOffset = m_iLayoutOffset;
	LayoutDesc.Format = _eFormat;
	LayoutDesc.InputSlot = 0;
	LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc.SemanticName = _strSemanticName;
	LayoutDesc.SemanticIndex = 0;
	m_vecLayoutInfo.push_back(LayoutDesc);

	m_iLayoutOffset += GetSizeofFormat(_eFormat);
}
