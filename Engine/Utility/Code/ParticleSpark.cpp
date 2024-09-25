#include "ParticleSpark.h"

CParticleSpark::CParticleSpark()
{
}

CParticleSpark::CParticleSpark(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CParticleSystem(_pGraphicDev)
{
}

CParticleSpark::CParticleSpark(const CParticleSpark& _rhs)
	: CParticleSystem(_rhs)
{
}

CParticleSpark::~CParticleSpark()
{
}

CParticleSpark* CParticleSpark::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CParticleSpark* pParticleSystem = new CParticleSpark(_pGraphicDev);

	if (FAILED(pParticleSystem->Ready_ParticleSystem()))
	{
		Safe_Release(pParticleSystem);
		MSG_BOX("particle system create failed");
		return nullptr;
	}

	return pParticleSystem;
}

CParticleSpark* CParticleSpark::Clone()
{
	return new CParticleSpark(*this);
}

void CParticleSpark::Reset_Particle(PARTICLEINFO* _pInfo)
{
	_pInfo->bIsAlive = TRUE;

	_pInfo->vPosition = { 0.f, 0.f, 0.f };

	_float fRadius = 0.1f;
	_vec3 vVelocity, vPosition;

	vPosition = Get_RandomVec3(_vec3(-1.f, -1.f, 0.f), _vec3(1.f, 1.f, 0.f));
	D3DXVec3Cross(&vPosition, &m_vDiraction, &vPosition);
	D3DXVec3Normalize(&vPosition, &vPosition);

	vVelocity = (vPosition * 2.f + m_vDiraction);
	D3DXVec3Normalize(&vVelocity, &vVelocity);
	vVelocity *= Get_RandomFloat(m_vSpeed.x, m_vSpeed.y);
	_pInfo->vVelocity = vVelocity;

	vPosition *= fRadius;
	_pInfo->vPosition = vPosition;

	//_pInfo->vAcceleration = -_pInfo->vVelocity * 1.f;
	_pInfo->vColor = m_tParam.vColor;
	_float fT = Get_RandomFloat(0.1f, 1.0f);
	//_pInfo->fLifeTime = m_tParam.fLifeTime * fT * fT * fT;
	_pInfo->fLifeTime = m_tParam.fLifeTime * fT * fT;
	_pInfo->fAge = 0.f;
}

void CParticleSpark::Free()
{
	CParticleSystem::Free();
}
