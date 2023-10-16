#include "pch.h"
#include "CItemScript.h"

#include "CPlayerScript.h"
#include "CWingScript.h"
CItemScript::CItemScript(ItemCode _item)
	: CScript(ITEMSCRIPT)
	, m_eItem(_item)
{
}

CItemScript::~CItemScript()
{
}

void CItemScript::Overlap(CCollider2D* _pOther)
{
	if (KEY_TAP(KEY::E))
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\item_pickup_01.wav");
		pSound->Play(1, 0.6f, true);
		if (m_eItem == ItemCode::shotgun)
		{
			_pOther->GetOwner()->GetChildObject()[0]->AddChild(CResMgr::GetInst()->FindRes<CPrefab>(L"shotgun")->Instantiate());
			Destroy();
		}
		else if (m_eItem == ItemCode::megahand)
		{
			_pOther->GetOwner()->GetChildObject()[0]->AddChild(CResMgr::GetInst()->FindRes<CPrefab>(L"megahand")->Instantiate());
			Destroy();
		}
		else if (m_eItem == ItemCode::waxwing)
		{
			_pOther->GetOwner()->GetScript<CPlayerScript>()->setFly(true);
			CGameObject* pwing = CResMgr::GetInst()->FindRes<CPrefab>(L"wing")->Instantiate();
			pwing->GetScript<CWingScript>()->setPlayer(_pOther->GetOwner());
			Instantiate(pwing, Vec3(0.f, 0.f, 0.f), 1);
			Destroy();
		}
		else if (m_eItem == ItemCode::scope)
		{
			_pOther->GetOwner()->GetScript<CPlayerScript>()->setSpread(0.6f);
			Destroy();
		}
	}
}

void CItemScript::SaveToFile(FILE* _File)
{
}

void CItemScript::LoadFromFile(FILE* _File)
{
}

