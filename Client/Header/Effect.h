#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CEffect
	: public Engine::CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffect();

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual	HRESULT Ready_GameObject();
	virtual	_int Update_GameObject(const _float& _fTimeDelta);
	virtual	void LateUpdate_GameObject();
	virtual	void Render_GameObject();

private:
	HRESULT Add_Component();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	_float m_fFrame;
};