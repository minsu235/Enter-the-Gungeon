#include "pch.h"
#include "CPlayerArmScript.h"

#include <Engine/CLevelMgr.h>
#include "CPlayerScript.h"

CPlayerArmScript::CPlayerArmScript()
	: CScript(PLAYERARMSCRIPT)
    , right(true)
{
}

CPlayerArmScript::~CPlayerArmScript()
{
}

float CPlayerArmScript::getMouseDIR()
{
    Vec3 vPos = GetOwner()->GetParent()->Transform()->GetWorldPos();
    vPos.y -= 30.f;
    Vec3 vMousePos = CLevelMgr::GetInst()->getMousepos();

    float degree = atan2((vMousePos.y - vPos.y), (vMousePos.x - vPos.x)) * 180 / 3.141592f;

    return degree;
}

void CPlayerArmScript::begin()
{
}

void CPlayerArmScript::tick()
{
    CGameObject* player = GetOwner()->GetParent();
    PLAYER_STATE player_state = player->GetScript<CPlayerScript>()->getPlayerState();
    if (player_state <= PLAYER_STATE::DODGE_BL || (PLAYER_STATE::SPIN <= player_state && player_state <= PLAYER_STATE::SLIDE_BL) ||
        (PLAYER_STATE::DODGEFALL_B <= player_state && player_state <= PLAYER_STATE::FALL_L))
    {
        MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\guide_hand_x.png"));
    }
    else
    {
        MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\guide_hand.png"));
    }

    Vec3 vRot;
    Vec3 vPos;
    vPos += m_offset;

    if (right)
    {
        vRot.z = getMouseDIR() / 180.f * 3.141592f;
        vPos += m_armpos;
        if (getMouseDIR() > 100.f || getMouseDIR() < -100.f)
            right = false;
    }
    else
    {
        if (getMouseDIR() > 0.f)
        {
            vRot.y = 180.f / 180.f * 3.141592f;
            vRot.z = -(180.f - getMouseDIR()) / 180.f * 3.141592f;
            vPos -= m_armpos;
        }
        else if (getMouseDIR() < 0.f)
        {
            vRot.y = 180.f / 180.f * 3.141592f;
            vRot.z = -(-180.f - getMouseDIR()) / 180.f * 3.141592f;
            vPos -= m_armpos;
        }

        if (getMouseDIR() < 80.f && getMouseDIR() > -80.f)
            right = true;
    }
    Transform()->SetRelativeRotation(vRot);
    if (vPos != Transform()->GetRelativePos())
        Transform()->SetRelativePos(vPos);
}

void CPlayerArmScript::SaveToFile(FILE* _File)
{
    CScript::SaveToFile(_File);
}

void CPlayerArmScript::LoadFromFile(FILE* _File)
{
    CScript::LoadFromFile(_File);
}

