#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CParticleSystem;
class CEffect;

END

class CEffectHeal : public CGameObject
{
private:
	explicit CEffectHeal(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectHeal();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectHeal* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT		Add_Component();
	void Set_ParticleParam();

public:
	static void OnOperate(void* _pParam);

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CParticleSystem* m_pParticleSystem;
	Engine::CEffect* m_pEffectCom;

};
