#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CParticleSystem;
class CEffect;

END

class CEffectExplosion : public CGameObject
{
private:
	explicit CEffectExplosion(LPDIRECT3DDEVICE9 pGrphicDev);
	virtual ~CEffectExplosion();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

private:
	HRESULT		Add_Component();

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pTextureSmokeCom;
	Engine::CParticleSystem* m_pParticleSystemSmoke;
	Engine::CParticleSystem* m_pParticleSystemFlare;
	Engine::CParticleSystem* m_pParticleSystemFragment;
	Engine::CEffect* m_pEffectCom;

public:
	static CEffectExplosion* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

public:
	static void OnOperate(void* _pParam);

private:
	void Set_FlareParticle();
	void Set_SmokeParticle();
	void Set_FragmentParticle();
};

