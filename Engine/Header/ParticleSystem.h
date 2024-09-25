#pragma once
#include "Component.h"

BEGIN(Engine)

class CTexture;

// vertex info
typedef struct tagParticle
{
	_vec3 vPosition;
	//_float fSize;
	D3DCOLOR dwColor;
	_vec2 vUV;
	static const DWORD FVF;

} PARTICLE, VTXPARTICLE;

typedef struct tagParticleInfo
{
	tagParticleInfo() { ZeroMemory(this, sizeof(tagParticleInfo)); }

	_vec3 vPosition;
	_vec3 vVelocity;
	_vec3 vAcceleration;
	_float fLifeTime;
	_float fAge;
	D3DCOLOR tColor;
	D3DCOLOR tColorFade;
	_bool bIsAlive;

} PARTICLEINFO;

typedef struct tagBoundingBox
{
	_vec3 vMin;
	_vec3 vMax;

} BOUNDINGBOX;


DWORD FtoDW(_float _f);
_float Get_RandomFloat(_float _fMin, _float _fMax);
_vec3 Get_RandomVec3(_vec3 _vMin, _vec3 _vMax);
_vec3 Get_RandomVec3(BOUNDINGBOX tBox);


class ENGINE_DLL CParticleSystem : public CComponent
{
public:
	typedef struct tagParam
	{
		tagParam() { ZeroMemory(this, sizeof(tagParam)); }

		BOUNDINGBOX tStartBoundary;
		_vec3 vInitVelocity;
		_vec3 vAcceleration;
		_float fLifeTime;
		D3DCOLOR tColor;
		D3DCOLOR tColorFade;

		_float fEmitRate;
		_float fSize;
		_vec3 vVelocityNoise;
		_int iMaxParticle;
		_float fGravity;
		BOUNDINGBOX tBoundary;

		_ulong dwAlphaRef;

	} PARAM;

public:
	enum class OPTION : _ulong
	{
		REPEAT,
		DEATH_OVER_TIME,
		DEATH_OVER_BOUNDARY,
		SIZE_OVER_TIME,
		COLOR_FADE,
		GRAVITY,
		ALPHATEST,
		ZWRITE,

		OPTION_END

	};

protected:
	explicit CParticleSystem();
	explicit CParticleSystem(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CParticleSystem(const CParticleSystem& _rhs);
	virtual ~CParticleSystem();

public:
	static CParticleSystem* Create(LPDIRECT3DDEVICE9 _pGraphic);

public:
	virtual HRESULT Ready_ParticleSystem();
	virtual _int Update_Component(const _float& _fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	void Render_Parcitle();

public:
	virtual CParticleSystem* Clone() override;

protected:
	virtual void Free() override;

private:
	void Set_PreRenderState();
	void Set_PostRenderState();

public:
	void Reset();
	virtual void Reset_Particle(PARTICLEINFO* _pInfo);

	void Add_Particle();
	_bool Is_Empty();
	_bool Is_Dead();

protected:
	void Remove_DeadParticles();

private:
	// static 으로 해도 될듯
	_float Compute_ParticleViewZ(const PARTICLEINFO& _tParticle, const _vec3& _vCameraPos);

public:
	void Set_InitVelocity(const _vec3& _vVelocity) { m_tParam.vInitVelocity = _vVelocity; }
	void Set_Parameter(const PARAM& _tParam) { m_tParam = _tParam; }
	void Set_Option(OPTION _eOption, _bool _value) { m_dwOptions = m_dwOptions | (_value ? 1 : 0) << (_ulong)_eOption; }

private:
	_bool Check_Option(OPTION _eOption) const { return m_dwOptions & 1 << (_uint)_eOption; }

protected:
	std::list<PARTICLEINFO> m_ParticleList;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	PARAM m_tParam;

	_ulong m_dwOptions;

	DWORD m_dwBufferSize;
	DWORD m_dwBufferOffset;
	DWORD m_dwbufferBatchSize;
};



END

