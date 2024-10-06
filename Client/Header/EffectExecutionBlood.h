#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CParticleSystem;
class CEffect;
class CRcTex;

END

class CEffectExecutionBlood : public CGameObject
{
private:
	enum class BLOOD
	{
		BULLKY,
		BULLKY2,
		FAN_SPREAD,
		//THIN_SPLATTER,
		//WHIP_LIKE,

		BLOOD_END
	};

	typedef struct tRenderInfo
	{
		BLOOD eBlood;
		_vec3 vPos;
		_uint iCurFrame;
	} RENDERINFO;

private:
	explicit CEffectExecutionBlood(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectExecutionBlood();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectExecutionBlood* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT		Add_Component();
	void Set_ParticleParam();
	void Set_UVGrid();

public:
	static void OnOperateFirst(void* _pParam);
	static void OnOperateSecond(void* _pParam);

private:
	Engine::CTransform* m_pTransformCom;

	Engine::CEffect* m_pEffectComFirst;
	Engine::CTransform* m_pTransformComFirst;
	Engine::CTexture* m_pTextureComFirst;
	Engine::CParticleSystem* m_pParticleSystem;


	Engine::CEffect* m_pEffectComSecond;
	Engine::CTransform* m_pTransformComSecond;
	Engine::CTransform* m_pTransformComArray[(_uint)BLOOD::BLOOD_END];
	Engine::CTexture* m_pTextureComSecond;
	//Engine::CRcTex* m_pBufferCom[(_uint)BLOOD::BLOOD_END];
	Engine::CRcTex* m_pBufferCom;

	std::array<_uint, (_uint)BLOOD::BLOOD_END> m_iTotalFrame;
	std::array<_uint, (_uint)BLOOD::BLOOD_END> m_iCurFrame;
	std::array<_vec2, (_uint)BLOOD::BLOOD_END> m_vGrid;

	std::array<_vec3, (_uint)BLOOD::BLOOD_END> m_vOffset;

	std::array<std::pair<BLOOD, _vec3>, 20> m_EmissionBuffer;

	float m_fEmissionInterval;
	_uint m_iCurEmmisionIdx;
};
