#include "pch.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CTransform.h"

CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_fMass(1.f)
	, m_fFriction(400.f)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::tick()
{
	if (m_vForce != Vec3(0.f, 0.f, 0.f))
	{
		int a = 0;
	}
	// F = M x A
	m_vAccel = m_vForce / m_fMass;

	// 속도에 가속도를 더한다.
	m_vVelocity += m_vAccel;

	// 마찰력 조건(적용된 힘이 없고, 속도가 0 이 아니고, 땅 위에 있을 때)
	if (m_vVelocity != Vec3(0.f, 0.f, 0.f))
	{
		// 속도의 반대방향으로 마찰력을 적용
		Vec3 vFriction = -m_vVelocity;
		vFriction = vFriction.Normalize() * m_fFriction * DT * m_fMass;

		// 마찰력으로 인한 속도 감소량이 현재 속도보다 더 큰 경우
		if (m_vVelocity.Length() < vFriction.Length())
		{
			// 속도를 0 로 만든다.
			m_vVelocity = Vec3(0.f, 0.f, 0.f);
		}
		else
		{
			// 속도에서 마찰력으로 인한 반대방향으로 속도를 차감한다.
			m_vVelocity += vFriction;
		}
	}

	// 속도에 맞게 물체를 이동시킨다.
	if (Transform() != nullptr)
	{
		Vec3 vPos = Transform()->GetRelativePos() + m_vVelocity * DT;
		Transform()->SetRelativePos(vPos);
	}

	m_vForce = Vec3(0.f, 0.f, 0.f);
}

void CRigidBody::finaltick()
{
}

void CRigidBody::UpdateData()
{
}

void CRigidBody::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	fwrite(&m_fMass, sizeof(float), 1, _File);
	fwrite(&m_fFriction, sizeof(float), 1, _File);
}

void CRigidBody::LoadFromFile(FILE* _File)
{
	fread(&m_fMass, sizeof(float), 1, _File);
	fread(&m_fFriction, sizeof(float), 1, _File);
}
