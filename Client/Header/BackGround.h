#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CBackGround
	: public Engine::CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CBackGround();

public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT	Add_Component();
	void Set_Animation();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CAnimator* m_pAnimator;

	_matrix	m_matView, m_matProj;
};