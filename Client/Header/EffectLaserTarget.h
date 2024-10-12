#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CEffect;
class CTransform;
class CTexture;
class CParticleSystem;

END

class CEffectLaserTarget : public CGameObject
{
private:
	explicit CEffectLaserTarget(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectLaserTarget();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectLaserTarget* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT Add_Component();
	void Set_ParticleCohesionPointParameter();
	void Set_ParticleSparkParameter();

public:
	static void OnOperate(void* _pParam);

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CEffect* m_pEffectCom;
	Engine::CParticleSystem* m_pParticleSystemComCohesionPoint;
	Engine::CParticleSystem* m_pParticleSystemComSpark;


private:
	_float m_fGraceTime;
	_bool m_bIsMissing;

	_bool m_bIsInGround;
};

