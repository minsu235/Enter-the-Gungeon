#include "pch.h"
#include "CWallScript.h"
#include "CMonsterBulletScript.h"

CWallScript::CWallScript()
	: CScript(WALLSCRIPT)
{
}

CWallScript::~CWallScript()
{
}

void CWallScript::tick()
{
}

void CWallScript::Overlap(CCollider2D* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player" || _pOther->GetOwner()->GetName() == L"Monster")
	{
		bool left = false, right = false, up = false, down = false;
		Vec2 Playerpos = _pOther->GetFinalPos();
		Vec2 Playerscale = _pOther->GetScale();

		Vec2 Otherpos = Vec2(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y);
		Vec2 Otherscale = Vec2(Transform()->GetRelativeScale().x, Transform()->GetRelativeScale().y);

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
	else if (_pOther->GetOwner()->GetName() == L"Bullet")
	{
		_pOther->Destroy();
	}
	else if (_pOther->GetOwner()->GetName() == L"MonsterBullet" && _pOther->GetOwner()->GetScript<CMonsterBulletScript>()->beFired())
	{
		_pOther->ReturnPool();
	}
}

void CWallScript::SaveToFile(FILE* _File)
{
}

void CWallScript::LoadFromFile(FILE* _File)
{
}
