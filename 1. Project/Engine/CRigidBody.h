#pragma once
#include "CComponent.h"
class CRigidBody :
    public CComponent
{
private:
	Vec3    m_vForce;
	Vec3    m_vAccel;
	Vec3    m_vVelocity;

	float   m_fMass;
	float   m_fFriction;

public:
	void AddForce(Vec3 _vF) { m_vForce += _vF; }
	void AddVelocity(Vec3 _vVelocity) { m_vVelocity += _vVelocity; }
	void SetVelocity(Vec3 _vVelocity) { m_vVelocity = _vVelocity; }
	Vec3 GetVelocity() { return m_vVelocity; }

	void SetMass(float _fMass) { m_fMass = _fMass; }

public:
	virtual void tick() override;
	virtual void finaltick() override;

	void UpdateData();

public:
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

	CLONE(CRigidBody);
public:
	CRigidBody();
	virtual ~CRigidBody();
};

