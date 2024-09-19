#include "ParticleSystem.h"

const DWORD PARTICLE::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE;

CParticleSystem::CParticleSystem()
{
}

CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
{
}

CParticleSystem::CParticleSystem(const CParticleSystem& _rhs)
	: CComponent(_rhs)
{
}

CParticleSystem::~CParticleSystem()
{
}

CParticleSystem* CParticleSystem::Create(LPDIRECT3DDEVICE9 _pGraphic)
{
	return nullptr;
}

HRESULT CParticleSystem::Ready_ParticleSystem()
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer
	(
		m_dwBufferSize * sizeof(VTXPARTICLE),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		VTXPARTICLE::FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		NULL
	), E_FAIL);

	return S_OK;
}

_int CParticleSystem::Update_Component(const _float& _fTimeDelta)
{
	return _int();
}

void CParticleSystem::LateUpdate_Component()
{
}

CParticleSystem* CParticleSystem::Clone()
{
	return nullptr;
}

void CParticleSystem::Free()
{
}

void CParticleSystem::Set_PreRenderState()
{
}

void CParticleSystem::Set_PostRenderState()
{
}

void CParticleSystem::Add_Particle()
{
}

_bool CParticleSystem::Is_Empty()
{
	return _bool();
}

_bool CParticleSystem::Is_Dead()
{
	return _bool();
}

void CParticleSystem::Remove_DeadParticles()
{
}

void CParticleSystem::Set_Texture(CTexture* _pTextureClass)
{
}

void CParticleSystem::Set_Texture(const vector<LPDIRECT3DTEXTURE9>& _vecTexture)
{
}
