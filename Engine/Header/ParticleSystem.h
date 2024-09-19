#pragma once
#include "Component.h"

BEGIN(Engine)

class CTexture;

// vertex info
typedef struct tagParticle
{
	_vec3 vPosition;
	D3DCOLOR dwColor;
	_float fSize;
	static const DWORD FVF;

} PARTICLE, VTXPARTICLE;

typedef struct tagParticleInfo
{
	_vec3 vPosition;
	_vec3 vVelocity;
	_vec3 vAcceleration;
	_float fLifeTime;
	_float fAge;
	_vec3 vColor;
	_vec3 vColorFade;
	_bool bIsAlive;

} PARTICLEINFO;

class ENGINE_DLL CParticleSystem : public CComponent
{
private:
	explicit CParticleSystem();
	explicit CParticleSystem(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CParticleSystem(const CParticleSystem& _rhs);
	virtual ~CParticleSystem();

public:
	static CParticleSystem* Create(LPDIRECT3DDEVICE9 _pGraphic);

public:
	HRESULT Ready_ParticleSystem();
	virtual _int Update_Component(const _float& _fTimeDelta) override;
	virtual void LateUpdate_Component();

public:
	virtual CParticleSystem* Clone() override;

private:
	virtual void Free() override;

	
public:
	void Set_PreRenderState();
	void Set_PostRenderState();

public:
	void Add_Particle();
	_bool Is_Empty();
	_bool Is_Dead();

protected:
	virtual void Remove_DeadParticles();

public:
	void Set_Texture(CTexture* _pTextureClass); // 아마 이게 주력으로 사용될듯
	void Set_Texture(const vector<LPDIRECT3DTEXTURE9>& _vecTexture);

private:
	vector<LPDIRECT3DTEXTURE9> m_vecTexture;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	_vec3 m_vOrigin;
	_float m_fEmitRate;
	_float m_fSize;
	std::list<PARTICLEINFO> m_ParticleList;
	_int m_iMaxParticle;


	DWORD m_dwBufferSize;
	DWORD m_dwBufferOffset;
	DWORD m_dwbufferBatchSize;
};

END
