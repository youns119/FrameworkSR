#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CEffect;
class CTransform;
class CTexture;
class CRcTex;

END

class CEffectFlash : public CGameObject
{
private:
	explicit CEffectFlash(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectFlash();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectFlash* Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec4& _vColor);

private:
	virtual void Free();

private:
	HRESULT Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CEffect* m_pEffectCom;

private:
	_vec4 m_vColor;
};

