#include "pch.h"
#include "CGameCameraScript.h"

#include <Engine/CLevelMgr.h>
#include "CPlayerScript.h"

CGameCameraScript::CGameCameraScript()
	: CScript(GAMECAMERASCRIPT)
	, m_player(nullptr)
	, m_fAcctime(0.f)
	, m_bCamerashake(false)
	, m_iAct(0)
	, m_bStart(true)
	, m_bBoss(false)
	, m_fSpeed(0.f)
{
}

CGameCameraScript::~CGameCameraScript()
{
}

void CGameCameraScript::BossCameraMove(Vec3 _bossPos)
{
	m_bBoss = true;
	m_bCamerashake = false;
	m_iAct = 0;
	m_fAcctime = 0.f;
	_bossPos.z = 0.f;
	m_fSpeed = Vec3::Distance(_bossPos, Transform()->GetRelativePos()) / 2.f;
	m_player->GetScript<CPlayerScript>()->PlayerStop(true);
}

void CGameCameraScript::begin()
{
}

void CGameCameraScript::tick()
{
	if (m_bStart)
	{
		m_fAcctime += DT;
		if (m_fAcctime >= 1.f)
		{
			m_bStart = false;
			m_fAcctime = 0.f;
		}
		return;
	}
	if (m_bBoss)
	{
		m_fAcctime += DT;
		if (m_fAcctime <= 2.f)
		{
			Vec3 vBoss = Vec3(4320.f, 2965.f, 0.f);
			vBoss -= Transform()->GetRelativePos();
			vBoss.z = 0.f;
			vBoss.Normalize();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += vBoss.x * m_fSpeed * DT;
			vPos.y += vBoss.y * m_fSpeed * DT;
			Transform()->SetRelativePos(vPos);
		}
		else if (m_fAcctime >= 7.f)
		{
			m_bBoss = false;
			m_fAcctime = 0.f;
			m_player->GetScript<CPlayerScript>()->PlayerStop(false);
		}
		else if (m_fAcctime >= 6.f)
		{
			Vec3 vPlayer = m_player->Transform()->GetRelativePos();
			vPlayer -= Transform()->GetRelativePos();
			vPlayer.z = 0.f;
			vPlayer.Normalize();
			Vec3 vPos = Transform()->GetRelativePos();
			vPos.x += vPlayer.x * m_fSpeed * 2.f * DT;
			vPos.y += vPlayer.y * m_fSpeed * 2.f * DT;
			Transform()->SetRelativePos(vPos);
		}
		return;
	}
	Move();

	if (m_bCamerashake)
	{
		m_fAcctime += DT;
		if (m_fAcctime > 0.02f && m_iAct == 0)
			++m_iAct;
		else if (m_fAcctime > 0.f && m_iAct == 0)
			Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(6.f, 0.f, 0.f));
		else if (m_fAcctime > 0.07f && m_iAct == 1)
			++m_iAct;
		else if (m_fAcctime > 0.05f && m_iAct == 1)
			Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(-6.f, 0.f, 0.f));
		else if (m_fAcctime > 0.1f && m_iAct == 2)
		{
			m_bCamerashake = false;
			m_iAct = 0;
			m_fAcctime = 0.f;
		}
		else if (m_fAcctime > 0.02f && m_iAct == 10)
			++m_iAct;
		else if (m_fAcctime > 0.f && m_iAct == 10)
			Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(4.f, 0.f, 0.f));
		else if (m_fAcctime > 0.07f && m_iAct == 11)
			++m_iAct;
		else if (m_fAcctime > 0.05f && m_iAct == 11)
			Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(-4.f, 0.f, 0.f));
		else if (m_fAcctime > 0.1f && m_iAct == 12)
		{
			m_bCamerashake = false;
			m_iAct = 0;
			m_fAcctime = 0.f;
		}
	}
}

void CGameCameraScript::Move()
{
	if (m_player == nullptr)
		return;

	Vec3 vPlayerPos = m_player->Transform()->GetRelativePos();
	vPlayerPos.y -= 20.f;

	Vec3 vMousePos = CLevelMgr::GetInst()->getMousepos();

	if (vPlayerPos.x - vMousePos.x > 925.f)
		vMousePos.x = vPlayerPos.x - 925.f;
	if (vPlayerPos.x - vMousePos.x < -925.f)
		vMousePos.x = vPlayerPos.x + 925.f;
	if (vPlayerPos.y - vMousePos.y > 520.f)
		vMousePos.y = vPlayerPos.y - 520.f;
	if (vPlayerPos.y - vMousePos.y < -520.f)
		vMousePos.y = vPlayerPos.y + 520.f;

	Vec3 vPos = Transform()->GetRelativePos();

	vPos.x = ((vPlayerPos.x * 3.5f) + vMousePos.x) / 4.5f;
	vPos.y = ((vPlayerPos.y * 3.5f) + vMousePos.y) / 4.5f;

	if (vPos != Transform()->GetRelativePos())
		Transform()->SetRelativePos(vPos);
}


