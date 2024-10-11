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
	//D3DCOLOR tColor;
	//D3DCOLOR tColorFade;
	_vec4 vColor;
	_vec4 vColorFade;
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
D3DCOLOR Vector_to_Color(_vec4 _vColor);


class ENGINE_DLL CParticleSystem : public CComponent
{
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
		ZWRITE_DISABLE,
		POINT_SCALE_DISABLE,
		EMISSION_CONTROL,
		ALPHA_SORT,
		ALPHAOP_ADD,

		OPTION_END

	};

public:
	enum class SHAPE
	{
		HEXAHEDRON,
		SPHERE,
		CIRCLE,

		SHAPE_END
	};

public:
	typedef struct tagParam
	{
		tagParam() { ZeroMemory(this, sizeof(tagParam)); }

		SHAPE eShape;

		union unionInit
		{
			unionInit() { tagHexahedron(); }
			struct tagHexahedron
			{
				tagHexahedron() { ZeroMemory(this, sizeof(tagHexahedron)); }
				BOUNDINGBOX tStartBoundary;
				_vec3 vInitVelocity;

			} tHexahedron;

			struct tagSphere
			{
				tagSphere() { ZeroMemory(this, sizeof(tagSphere)); }
				_vec3 vStartPos;
				_float fRadius;
				_float fPhi;
				_float fTheta;

			} tSphere;

			struct tagCircle
			{
				tagCircle() { ZeroMemory(this, sizeof(tagCircle)); }
				_vec3 vStartPos;
				_float fHeight;
				_float fRadius;

			} tCircle;

		} tInit;

		_vec3 vVelocityNoise;
		_vec3 vAcceleration;
		_float fLifeTime;
		_vec4 vColor;
		_vec4 vColorFade;

		_float fEmitRate;
		_uint iEmitCnt;
		_float fSize;
		_float fSizeFade;
		_uint iTotalCnt;
		_float fGravity;
		BOUNDINGBOX tBoundary;

		_ulong dwAlphaRef;

	} PARAM;

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
	void SetUp_Particle();

protected:
	void Remove_DeadParticles();

private:
	// static 으로 해도 될듯
	_float Compute_ParticleViewZ(const PARTICLEINFO& _tParticle, const _vec3& _vCameraPos);

public:
	void Set_InitVelocity(const _vec3& _vVelocity) { m_tParam.tInit.tHexahedron.vInitVelocity = _vVelocity; }
	void Set_Parameter(const PARAM& _tParam) { m_tParam = _tParam; }
	void Set_Option(OPTION _eOption, _bool _value) { m_bOptionArray[(_uint)_eOption] = _value; }

	const PARAM& Get_Parameter() { return m_tParam; }

private:
	_bool Check_Option(OPTION _eOption) const { return m_bOptionArray[(_uint)_eOption]; }

protected:
	std::list<PARTICLEINFO> m_ParticleList;
	std::list<PARTICLEINFO> m_ReadyParticleList;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	PARAM m_tParam;
	std::array<_bool, (_uint)OPTION::OPTION_END> m_bOptionArray;
	_float m_fEmitTime;
	_float m_fParticleSize;

	DWORD m_dwBufferSize;
	DWORD m_dwBufferOffset;
	DWORD m_dwbufferBatchSize;
};



END

