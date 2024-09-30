#pragma once
#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CComponentCamera : public CComponent
{
protected:
	explicit CComponentCamera(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CComponentCamera();

public:
	virtual	HRESULT	Ready_ComponentCamera();
	virtual	_int Update_Component(const _float& _fTimeDelta);
	virtual	void LateUpdate_Component();
	void SetFov(_float pFov) { m_fFov = pFov; }
public:
	static CComponentCamera* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual CComponentCamera* Clone();

protected:
	virtual void Free();

protected:
	_vec3 m_vEye, m_vAt, m_vUp;
	_float m_fFov, m_fAspect, m_fNear, m_fFar;
	_matrix m_matView, m_matProj;
};

END
