#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera 
	: public CGameObject
{
protected :
	explicit CCamera(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CCamera();

public :
	virtual	HRESULT	Ready_GameObject();
	virtual	_int Update_GameObject(const _float& _fTimeDelta);
	virtual	void LateUpdate_GameObject();

protected :
	virtual void Free();

protected :
	_vec3 m_vEye, m_vAt, m_vUp;
	_float m_fFov, m_fAspect, m_fNear, m_fFar;
	_matrix m_matView, m_matProj;
};

END