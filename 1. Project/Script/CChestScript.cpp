#include "pch.h"
#include "CChestScript.h"

#include "CItemScript.h"

CChestScript::CChestScript(ItemCode _item)
	: CScript(CHESTSCRIPT)
	, m_eItem(_item)
	, m_bOpened(false)
	, m_first(false)
{
}

CChestScript::~CChestScript()
{
}

void CChestScript::tick()
{
	if (!m_first)
	{
		m_first = true;
		Animator2D()->Play(L"IDLE", false);
	}
}

void CChestScript::BeginOverlap(CCollider2D* _pOther)
{
	if(!m_bOpened)
		Animator2D()->Play(L"IDLE_white", false);
}

void CChestScript::Overlap(CCollider2D* _pOther)
{
	if (!m_bOpened)
	{
		if (KEY_TAP(KEY::E))
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\chest_open_01.wav");
			pSound->Play(1, 0.6f, true);
			m_bOpened = true;
			Animator2D()->Play(L"OPEN", false);
			CGameObject* pObject = new CGameObject;
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);
			pObject->AddComponent(new CCollider2D);
			pObject->AddComponent(new CRigidBody);
			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
			pObject->RigidBody()->AddForce(Vec3(0.f, -300.f, 0.f));
			if (m_eItem == ItemCode::shotgun)
			{
				pObject->AddComponent(new CItemScript);
				pObject->Transform()->SetRelativeScale(Vec3(72.f, 15.f, 0.f));
				pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemShotgunMtrl"));
				pObject->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\shotgun_idle_001.png"));
			}
			else if (m_eItem == ItemCode::megahand)
			{
				pObject->AddComponent(new CItemScript(ItemCode::megahand));
				pObject->Transform()->SetRelativeScale(Vec3(30.f, 25.f, 0.f));
				pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemMegahandMtrl"));
				pObject->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\mega_buster_idle_001.png"));
			}
			else if (m_eItem == ItemCode::scope)
			{
				pObject->AddComponent(new CItemScript(ItemCode::scope));
				pObject->Transform()->SetRelativeScale(Vec3(42.f, 34.f, 0.f));
				pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemScopeMtrl"));
				pObject->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\scope_001.png"));
			}
			else if (m_eItem == ItemCode::waxwing)
			{
				pObject->AddComponent(new CItemScript(ItemCode::waxwing));
				pObject->Transform()->SetRelativeScale(Vec3(39.f, 39.f, 0.f));
				pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemWaxwingMtrl"));
				pObject->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\gun\\wax_feather_001.png"));
			}
			Instantiate(pObject, Transform()->GetRelativePos(), 6);
		}
	}
		
}

void CChestScript::EndOverlap(CCollider2D* _pOther)
{
	if (!m_bOpened)
		Animator2D()->Play(L"IDLE", false);
}

void CChestScript::SaveToFile(FILE* _File)
{
}

void CChestScript::LoadFromFile(FILE* _File)
{
}