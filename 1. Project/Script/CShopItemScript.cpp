#include "pch.h"
#include "CShopItemScript.h"
#include "CPlayerScript.h"

CShopItemScript::CShopItemScript(ItemCode _item)
	: CScript(SHOPITEMSCRIPT)
	, m_eItem(_item)
	, m_iPrice(0)
	, m_bSoldout(false)
	, m_pTag(nullptr)
{
	switch (_item)
	{
	case ItemCode::megahand:
		m_iPrice = 90;
		break;
	case ItemCode::scope:
		m_iPrice = 48;
		break;
	case ItemCode::heart:
		m_iPrice = 30;
		break;
	case ItemCode::blank:
		m_iPrice = 20;
		break;
	}
}

CShopItemScript::~CShopItemScript()
{
}

void CShopItemScript::tick()
{
}

void CShopItemScript::BeginOverlap(CCollider2D* _pOther)
{
	if (!m_bSoldout)
	{
		CGameObject* pTag = new CGameObject;
		pTag->AddComponent(new CTransform);
		pTag->AddComponent(new CMeshRender);

		pTag->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += 40.f;
		vPos.z = 1.f;
		
		switch (m_eItem)
		{
		case ItemCode::megahand:
			pTag->Transform()->SetRelativeScale(Vec3(235.f, 33.f, 0.f));
			pTag->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"shop_megahandMtrl"));
			vPos.x += 118.f;
			Instantiate(pTag, vPos, 0);
			break;
		case ItemCode::scope:
			pTag->Transform()->SetRelativeScale(Vec3(170.f, 33.f, 0.f));
			pTag->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"shop_scopeMtrl"));
			vPos.x += 85.f;
			Instantiate(pTag, vPos, 0);
			break;
		case ItemCode::blank:
			pTag->Transform()->SetRelativeScale(Vec3(176.f, 33.f, 0.f));
			pTag->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"shop_blankMtrl"));
			vPos.x += 88.f;
			Instantiate(pTag, vPos, 0);
			break;
		case ItemCode::heart:
			pTag->Transform()->SetRelativeScale(Vec3(170.f, 33.f, 0.f));
			pTag->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"shop_heartMtrl"));
			vPos.x += 85.f;
			Instantiate(pTag, vPos, 0);
			break;
		}

		m_pTag = pTag;
	}
}

void CShopItemScript::Overlap(CCollider2D* _pOther)
{
	if (!m_bSoldout)
	{
		if (KEY_TAP(KEY::E))
		{
			if (_pOther->GetOwner()->GetScript<CPlayerScript>()->addGold(-1 * m_iPrice))
			{
				m_bSoldout = true;
				MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemSoldOutMtrl"));
				Transform()->SetRelativeScale(Vec3(42.f, 34.f, 0.f));
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\item_purchase_01.wav");
				pSound->Play(1, 0.6f, true);
				if (m_pTag != nullptr)
				{
					m_pTag->Destroy();
					m_pTag = nullptr;
				}
				switch (m_eItem)
				{
				case ItemCode::megahand:
					_pOther->GetOwner()->GetChildObject()[0]->AddChild(CResMgr::GetInst()->FindRes<CPrefab>(L"megahand")->Instantiate());
					break;
				case ItemCode::scope:
					_pOther->GetOwner()->GetScript<CPlayerScript>()->setSpread(0.6f);
					break;
				case ItemCode::blank:
					_pOther->GetOwner()->GetScript<CPlayerScript>()->addBlank();
					break;
				case ItemCode::heart:
					_pOther->GetOwner()->GetScript<CPlayerScript>()->addHP(2);
					break;
				}
			}
			else
			{
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\purchase_unable_01.wav");
				pSound->Play(1, 0.6f, false);
			}
		}
	}
}

void CShopItemScript::EndOverlap(CCollider2D* _pOther)
{
	if (!m_bSoldout)
	{
		if (m_pTag != nullptr)
		{
			m_pTag->Destroy();
			m_pTag = nullptr;
		}
	}
	else
	{
		if (Collider2D()->getOverlapCount() == 0)
		{
			GetOwner()->DelComponent(COMPONENT_TYPE::COLLIDER2D);
		}
	}
}

void CShopItemScript::SaveToFile(FILE* _File)
{
}

void CShopItemScript::LoadFromFile(FILE* _File)
{
}
