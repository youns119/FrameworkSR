#pragma once
#include "ParticleSystem.h"

BEGIN(Engine)

class ENGINE_DLL CParticleSpark : public CParticleSystem
{
private:
	explicit CParticleSpark();
	explicit CParticleSpark(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticleSpark(const CParticleSpark& rhs);
	virtual ~CParticleSpark();

public:
	static CParticleSpark* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual CParticleSpark* Clone() override;


public:
	virtual void Reset_Particle(PARTICLEINFO* _pInfo) override;

private:
	virtual void Free() override;

public:
	void Set_Diraction(const _vec3& _vDir) { D3DXVec3Normalize(&m_vDiraction, &_vDir); }
	void Set_Speed(const _vec2& _vSpeed) { m_vSpeed = _vSpeed; }

private:
	_vec3 m_vDiraction;
	_vec2 m_vSpeed;

};


END
