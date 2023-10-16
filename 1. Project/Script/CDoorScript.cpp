#include "pch.h"
#include "CDoorScript.h"
#include "CGameCameraScript.h"
#include <Engine/CRenderMgr.h>

CDoorScript::CDoorScript()
	: CScript(DOORSCRIPT)
	, m_first(true)
	, m_bBossdoor(false)
	, m_fAcctime(0.f)
	, m_bOpen(false)
{
}

CDoorScript::~CDoorScript()
{
}

void CDoorScript::tick()
{
	if (m_first)
	{
		Animator2D()->Play(L"IDLE", false);
		m_first = false;
	}

	if (m_bOpen)
	{
		m_fAcctime += DT;
		CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->camerashake_bossdoor();
		if (m_fAcctime > 1.5f)
		{
			m_bOpen = false;
			CGameObject* pfire = CResMgr::GetInst()->FindRes<CPrefab>(L"cartoonfire")->Instantiate();
			pfire->Animator2D()->Play(L"IDLE", true);
			GetOwner()->AddChild(pfire);
			CGameObject* pPLight = new CGameObject;
			pPLight->SetName(L"light");

			pPLight->AddComponent(new CTransform);
			pPLight->AddComponent(new CLight2D);

			pPLight->Light2D()->SetLightColor(Vec3(1.f, 0.4f, 0.4f));
			pPLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
			pPLight->Light2D()->SetRadius(100.f);
			GetOwner()->AddChild(pPLight);
		}
	}
}

void CDoorScript::BeginOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		Animator2D()->Play(L"OPEN", false);
		if (m_bBossdoor)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bossdoor_open_01.wav");
			pSound->Play(1, 0.9f, true);
			m_bOpen = true;
		}
		else
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\opendoor.mp3");
			pSound->Play(1, 0.6f, true);
		}
	}
	if (_pOther->GetOwner()->GetName() == L"Bullet")
	{
		_pOther->Destroy();
	}
}

void CDoorScript::Overlap(CCollider2D* _pOther)
{
}

void CDoorScript::EndOverlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		GetOwner()->DelComponent(COMPONENT_TYPE::COLLIDER2D);
	}
}

void CDoorScript::SaveToFile(FILE* _File)
{
}

void CDoorScript::LoadFromFile(FILE* _File)
{
}


