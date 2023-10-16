#include "pch.h"
#include "CMonsterArm.h"

#include "CShotgunBulletKinScript.h"
#include "CBulletKinScript.h"

CMonsterArm::CMonsterArm()
	: CScript(MONSTERARM)
	, right(true)
	, m_type(Monster_type::magnum)
{
}

CMonsterArm::~CMonsterArm()
{
}

float CMonsterArm::getPlayerDegree()
{
	Vec3 playerpos;
	if (m_type == Monster_type::shotgun)
	{
		CGameObject* pPlayer = GetOwner()->GetParent()->GetScript<CShotgunBulletKinScript>()->getPlayer();
		if(pPlayer != nullptr)
			playerpos = GetOwner()->GetParent()->GetScript<CShotgunBulletKinScript>()->getPlayer()->Transform()->GetWorldPos();
	}
    else if (m_type == Monster_type::magnum)
    {
        CGameObject* pPlayer = GetOwner()->GetParent()->GetScript<CBulletKinScript>()->getPlayer();
        if (pPlayer != nullptr)
            playerpos = GetOwner()->GetParent()->GetScript<CBulletKinScript>()->getPlayer()->Transform()->GetWorldPos();
    }
	playerpos.y -= 30.f;
	playerpos.z = 0.f;
	Vec3 monsterpos = Transform()->GetWorldPos();
	Vec3 vDIR = playerpos - monsterpos;
	float degree = atan2((playerpos.y - monsterpos.y), (playerpos.x - monsterpos.x)) * 180 / 3.141592f;
	return degree;
}

void CMonsterArm::tick()
{
    CGameObject* player = GetOwner()->GetParent();
    if (m_type == Monster_type::shotgun)
    {
        if (player->GetScript<CShotgunBulletKinScript>()->getState() == ShotgunBulletKin_STATE::FALL || player->GetScript<CShotgunBulletKinScript>()->getState() == ShotgunBulletKin_STATE::DEATH)
        {
            Destroy();
        }
    }
    else if (m_type == Monster_type::magnum)
    {
        if (player->GetScript<CBulletKinScript>()->getState() == BulletKin_STATE::FALL || player->GetScript<CBulletKinScript>()->getState() == BulletKin_STATE::DEATH)
        {
            Destroy();
        }
    }

    Vec3 vRot;
    Vec3 vPos;
    vPos += m_offset;

    if (right)
    {
        vRot.z = getPlayerDegree() / 180.f * 3.141592f;
        vPos += m_armpos;
        if (getPlayerDegree() > 100.f || getPlayerDegree() < -100.f)
            right = false;
    }
    else
    {
        if (getPlayerDegree() > 0.f)
        {
            vRot.y = 180.f / 180.f * 3.141592f;
            vRot.z = -(180.f - getPlayerDegree()) / 180.f * 3.141592f;
            vPos -= m_armpos;
        }
        else if (getPlayerDegree() < 0.f)
        {
            vRot.y = 180.f / 180.f * 3.141592f;
            vRot.z = -(-180.f - getPlayerDegree()) / 180.f * 3.141592f;
            vPos -= m_armpos;
        }

        if (getPlayerDegree() < 80.f && getPlayerDegree() > -80.f)
            right = true;
    }
    Transform()->SetRelativeRotation(vRot);
    if (vPos != Transform()->GetRelativePos())
        Transform()->SetRelativePos(vPos);
}

void CMonsterArm::SaveToFile(FILE* _File)
{
}

void CMonsterArm::LoadFromFile(FILE* _File)
{
}

