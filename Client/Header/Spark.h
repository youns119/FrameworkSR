#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CParticleSpark;
class CEffect;

END

class CSpark : public CGameObject
{
private:
	explicit CSpark(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CSpark();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CSpark* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	Engine::CParticleSpark* m_pParticleSystem;
	Engine::CEffect* m_pEffectCom;

};
