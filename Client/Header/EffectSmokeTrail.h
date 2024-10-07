#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CEffect;
class CTransform;
class CTexture;
class CRcTex;
class CParticleSystem;

END

class CEffectSmokeTrail : public CGameObject
{
private:
	explicit CEffectSmokeTrail(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectSmokeTrail();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectSmokeTrail* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT Add_Component();
	void Set_ParticleParameter();

public:
	static void OnOperate(void* _pParam);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CEffect* m_pEffectCom;
	Engine::CParticleSystem* m_pParticleSystemCom;


private:
	_float m_fGraceTime;
	_bool m_bIsMissing;
};

