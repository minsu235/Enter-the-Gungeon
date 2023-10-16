#include "pch.h"
#include "CTableScript.h"
#include "CBulletScript.h"
#include "CMonsterBulletScript.h"

CTableScript::CTableScript()
	: CScript(TABLESCRIPT)
	, m_eState(Table_State::IDLE)
	, m_iHP(6)
	, m_bFirst(true)
{
}

CTableScript::~CTableScript()
{
}

void CTableScript::tick()
{
	if (m_bFirst)
	{
		Animator2D()->Play(L"IDLE", false);
		m_bFirst = false;
	}
}

void CTableScript::Overlap(CCollider2D* _pOther)
{
	if (m_eState == Table_State::IDLE)
	{
		if (_pOther->GetOwner()->GetName() == L"Player")
		{
			Vec2 Playerpos = _pOther->GetFinalPos();
			Vec2 Tablepos = Collider2D()->GetFinalPos();
			if (Tablepos.y + 41.f < Playerpos.y)
			{
				Animator2D()->Play(L"IDLE_B", false);
			}
			else if (Tablepos.y - 41.f > Playerpos.y)
			{
				Animator2D()->Play(L"IDLE_F", false);
			}
			else if (Tablepos.x - 27.f > Playerpos.x)
			{
				Animator2D()->Play(L"IDLE_L", false);
			}
			else if (Tablepos.x + 27.f < Playerpos.x)
			{
				Animator2D()->Play(L"IDLE_R", false);
			}

			if (KEY_TAP(KEY::E))
			{
				if (Tablepos.y + 41.f < Playerpos.y)
				{
					Animator2D()->Play(L"Flip_up", false);
					Collider2D()->SetOffsetPos(Vec2(0.f, -32.f));
					Collider2D()->SetScale(Vec2(54.f, 65.f));
					m_eState = Table_State::F;
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\table_flip_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.6f, true);
				}
				else if (Tablepos.y - 41.f > Playerpos.y)
				{
					Animator2D()->Play(L"Flip_down", false);
					Collider2D()->SetOffsetPos(Vec2(0.f, 68.f));
					Collider2D()->SetScale(Vec2(54.f, 65.f));
					m_eState = Table_State::B;
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\table_flip_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.6f, true);
				}
				else if (Tablepos.x - 27.f > Playerpos.x)
				{
					Animator2D()->Play(L"Flip_left", false);
					Collider2D()->SetOffsetPos(Vec2(30.f, 0.f));
					Collider2D()->SetScale(Vec2(15.f, 100.f));
					m_eState = Table_State::R;
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\table_flip_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.6f, true);
				}
				else if (Tablepos.x + 27.f < Playerpos.x)
				{
					Animator2D()->Play(L"Flip_right", false);
					Collider2D()->SetOffsetPos(Vec2(-30.f, 0.f));
					Collider2D()->SetScale(Vec2(15.f, 100.f));
					m_eState = Table_State::L;
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\table_flip_0" + to_wstring((rand() % 3) + 1) + L".wav");
					pSound->Play(1, 0.6f, true);
				}
			}
		}

		if (_pOther->GetOwner()->GetName() == L"Player" || _pOther->GetOwner()->GetName() == L"Monster" || _pOther->GetOwner()->GetName() == L"Table")
		{
			bool left = false, right = false, up = false, down = false;
			Vec2 Playerpos = _pOther->GetFinalPos();
			Vec2 Playerscale = _pOther->GetScale();

			Vec2 Otherpos = Collider2D()->GetFinalPos();
			Vec2 Otherscale = Vec2(54.f, 83.f);

			float t, b, l, r;
			//겹쳐진 사각형 y 길이 구하기
			// top
			if (Playerpos.y + Playerscale.y / 2 >= Otherpos.y + Otherscale.y / 2)
			{
				t = Otherpos.y + Otherscale.y / 2;
				up = true;
			}
			else
			{
				t = Playerpos.y + Playerscale.y / 2;
				down = true;
			}
			// bottom
			if (Playerpos.y - Playerscale.y / 2 >= Otherpos.y - Otherscale.y / 2)
			{
				b = Playerpos.y - Playerscale.y / 2;
				up = true;
			}
			else
			{
				b = Otherpos.y - Otherscale.y / 2;
				down = true;
			}
			//겹쳐진 사각형 x 길이 구하기
			// right
			if (Playerpos.x + Playerscale.x / 2 >= Otherpos.x + Otherscale.x / 2)
			{
				r = Otherpos.x + Otherscale.x / 2;
				right = true;
			}
			else
			{
				r = Playerpos.x + Playerscale.x / 2;
				left = true;
			}
			// left
			if (Playerpos.x - Playerscale.x / 2 >= Otherpos.x - Otherscale.x / 2)
			{
				l = Playerpos.x - Playerscale.x / 2;
				right = true;
			}
			else
			{
				l = Otherpos.x - Otherscale.x / 2;
				left = true;
			}

			if (r - l > 0.f && t - b > 0.f)
			{
				if (r - l >= t - b)
				{
					right = false;
					left = false;
				}
				else
				{
					up = false;
					down = false;
				}

				Vec3 vPos = _pOther->Transform()->GetRelativePos();

				if (up)
				{
					vPos.y += t - b + 1;
				}
				if (down)
				{
					vPos.y -= t - b + 1;
				}
				if (left)
				{
					vPos.x -= r - l + 1;
				}
				if (right)
				{
					vPos.x += r - l + 1;
				}

				if (vPos != Transform()->GetRelativePos())
					_pOther->Transform()->SetRelativePos(vPos);
			}
		}
	}
	else
	{
		if (_pOther->GetOwner()->GetName() == L"Player" || _pOther->GetOwner()->GetName() == L"Monster"	|| (_pOther->GetOwner()->GetName() == L"Table" && _pOther->GetOwner()->GetScript<CTableScript>()->m_eState != Table_State::IDLE))
		{
			bool left = false, right = false, up = false, down = false;
			Vec2 Playerpos = _pOther->GetFinalPos();
			Vec2 Playerscale = _pOther->GetScale();

			Vec2 Otherpos = Collider2D()->GetFinalPos();
			Vec2 Otherscale = Collider2D()->GetScale();

			float t, b, l, r;
			//겹쳐진 사각형 y 길이 구하기
			// top
			if (Playerpos.y + Playerscale.y / 2 >= Otherpos.y + Otherscale.y / 2)
			{
				t = Otherpos.y + Otherscale.y / 2;
				up = true;
			}
			else
			{
				t = Playerpos.y + Playerscale.y / 2;
				down = true;
			}
			// bottom
			if (Playerpos.y - Playerscale.y / 2 >= Otherpos.y - Otherscale.y / 2)
			{
				b = Playerpos.y - Playerscale.y / 2;
				up = true;
			}
			else
			{
				b = Otherpos.y - Otherscale.y / 2;
				down = true;
			}
			//겹쳐진 사각형 x 길이 구하기
			// right
			if (Playerpos.x + Playerscale.x / 2 >= Otherpos.x + Otherscale.x / 2)
			{
				r = Otherpos.x + Otherscale.x / 2;
				right = true;
			}
			else
			{
				r = Playerpos.x + Playerscale.x / 2;
				left = true;
			}
			// left
			if (Playerpos.x - Playerscale.x / 2 >= Otherpos.x - Otherscale.x / 2)
			{
				l = Playerpos.x - Playerscale.x / 2;
				right = true;
			}
			else
			{
				l = Otherpos.x - Otherscale.x / 2;
				left = true;
			}

			if (r - l >= t - b)
			{
				right = false;
				left = false;
			}
			else
			{
				up = false;
				down = false;
			}

			Vec3 vPos = _pOther->Transform()->GetRelativePos();
			Vec3 vTablepos = Transform()->GetRelativePos();
			if (up)
			{
				vPos.y += t - b + 1;
				vTablepos.y -= 300.f * DT;
			}
			if (down)
			{
				vPos.y -= t - b + 1;
				vTablepos.y += 300.f * DT;
			}
			if (left)
			{
				vPos.x -= r - l + 1;
				vTablepos.x += 300.f * DT;
			}
			if (right)
			{
				vPos.x += r - l + 1;
				vTablepos.x -= 300.f * DT;
			}

			if (vPos != Transform()->GetRelativePos())
				_pOther->Transform()->SetRelativePos(vPos);
			Transform()->SetRelativePos(vTablepos);
		}
		else if (_pOther->GetOwner()->GetName() == L"Bullet" || (_pOther->GetOwner()->GetName() == L"MonsterBullet" && _pOther->GetOwner()->GetScript<CMonsterBulletScript>()->beFired()))
		{
			m_iHP -= 1;
			if (_pOther->GetOwner()->GetName() == L"Bullet")
			{
				_pOther->GetOwner()->GetScript<CBulletScript>()->getDMG();
				_pOther->Destroy();
			}
			else
				_pOther->ReturnPool();
			
			if (m_iHP == 4)
			{
				switch (m_eState)
				{
				case Table_State::L:
					Animator2D()->Play(L"Break1_L", false);
					break;
				case Table_State::R:
					Animator2D()->Play(L"Break1_R", false);
					break;
				case Table_State::F:
					Animator2D()->Play(L"Break1_F", false);
					break;
				case Table_State::B:
					Animator2D()->Play(L"Break1_B", false);
					break;
				}
			}
			else if (m_iHP == 2)
			{
				switch (m_eState)
				{
				case Table_State::L:
					Animator2D()->Play(L"Break2_L", false);
					break;
				case Table_State::R:
					Animator2D()->Play(L"Break2_R", false);
					break;
				case Table_State::F:
					Animator2D()->Play(L"Break2_F", false);
					break;
				case Table_State::B:
					Animator2D()->Play(L"Break2_B", false);
					break;
				}
			}
			else if (m_iHP == 0)
			{
				switch (m_eState)
				{
				case Table_State::L:
					Animator2D()->Play(L"Break3_L", false);
					break;
				case Table_State::R:
					Animator2D()->Play(L"Break3_R", false);
					break;
				case Table_State::F:
					Animator2D()->Play(L"Break3_F", false);
					break;
				case Table_State::B:
					Animator2D()->Play(L"Break3_B", false);
					break;
				}
				Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\barrel_break_0" + to_wstring((rand() % 3) + 1) + L".wav");
				pSound->Play(1, 0.1f, true);
				GetOwner()->DelComponent(COMPONENT_TYPE::COLLIDER2D);
			}
		}
		else if (_pOther->GetOwner()->GetName() == L"Wall")
		{
			bool left = false, right = false, up = false, down = false;
			Vec2 Playerpos = Collider2D()->GetFinalPos();
			Vec2 Playerscale = Collider2D()->GetScale();

			Vec2 Otherpos = Vec2(_pOther->GetOwner()->GetParent()->Transform()->GetRelativePos().x + _pOther->GetOwner()->Transform()->GetRelativePos().x,
				_pOther->GetOwner()->GetParent()->Transform()->GetRelativePos().y + _pOther->GetOwner()->Transform()->GetRelativePos().y);
			Vec2 Otherscale = Vec2(_pOther->GetOwner()->Transform()->GetRelativeScale().x, _pOther->GetOwner()->Transform()->GetRelativeScale().y);

			float t, b, l, r;
			//겹쳐진 사각형 y 길이 구하기
			// top
			if (Playerpos.y + Playerscale.y / 2 >= Otherpos.y + Otherscale.y / 2)
			{
				t = Otherpos.y + Otherscale.y / 2;
				up = true;
			}
			else
			{
				t = Playerpos.y + Playerscale.y / 2;
				down = true;
			}
			// bottom
			if (Playerpos.y - Playerscale.y / 2 >= Otherpos.y - Otherscale.y / 2)
			{
				b = Playerpos.y - Playerscale.y / 2;
				up = true;
			}
			else
			{
				b = Otherpos.y - Otherscale.y / 2;
				down = true;
			}
			//겹쳐진 사각형 x 길이 구하기
			// right
			if (Playerpos.x + Playerscale.x / 2 >= Otherpos.x + Otherscale.x / 2)
			{
				r = Otherpos.x + Otherscale.x / 2;
				right = true;
			}
			else
			{
				r = Playerpos.x + Playerscale.x / 2;
				left = true;
			}
			// left
			if (Playerpos.x - Playerscale.x / 2 >= Otherpos.x - Otherscale.x / 2)
			{
				l = Playerpos.x - Playerscale.x / 2;
				right = true;
			}
			else
			{
				l = Otherpos.x - Otherscale.x / 2;
				left = true;
			}

			if (r - l >= t - b)
			{
				right = false;
				left = false;
			}
			else
			{
				up = false;
				down = false;
			}

			Vec3 vPos = Transform()->GetRelativePos();

			if (up)
			{
				vPos.y += t - b + 1;
			}
			if (down)
			{
				vPos.y -= t - b + 1;
			}
			if (left)
			{
				vPos.x -= r - l + 1;
			}
			if (right)
			{
				vPos.x += r - l + 1;
			}

			if (vPos != Transform()->GetRelativePos())
				Transform()->SetRelativePos(vPos);
		}
	}
}

void CTableScript::EndOverlap(CCollider2D* _pOther)
{
	if (m_eState == Table_State::IDLE)
	{
		if (_pOther->GetOwner()->GetName() == L"Player")
		{
			Animator2D()->Play(L"IDLE", false);
		}
	}
}

void CTableScript::SaveToFile(FILE* _File)
{
}

void CTableScript::LoadFromFile(FILE* _File)
{
}