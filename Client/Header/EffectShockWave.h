#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CEffect;

END

class CEffectShockWave : public CGameObject
{
private:
	explicit CEffectShockWave(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectShockWave();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectShockWave* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT		Add_Component();
	static void OnOperate(void* _pParam);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CEffect* m_pEffectCom;

	_vec3 m_vMaxSize;
	_vec3 m_vMinSize;

	_vec4 m_vInitColor;
	_vec4 m_vFadeColor;
};
