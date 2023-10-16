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

	// �ӵ��� ���ӵ��� ���Ѵ�.
	m_vVelocity += m_vAccel;

	// ������ ����(����� ���� ����, �ӵ��� 0 �� �ƴϰ�, �� ���� ���� ��)
	if (m_vVelocity != Vec3(0.f, 0.f, 0.f))
	{
		// �ӵ��� �ݴ�������� �������� ����
		Vec3 vFriction = -m_vVelocity;
		vFriction = vFriction.Normalize() * m_fFriction * DT * m_fMass;

		// ���������� ���� �ӵ� ���ҷ��� ���� �ӵ����� �� ū ���
		if (m_vVelocity.Length() < vFriction.Length())
		{
			// �ӵ��� 0 �� �����.
			m_vVelocity = Vec3(0.f, 0.f, 0.f);
		}
		else
		{
			// �ӵ����� ���������� ���� �ݴ�������� �ӵ��� �����Ѵ�.
			m_vVelocity += vFriction;
		}
	}

	// �ӵ��� �°� ��ü�� �̵���Ų��.
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
