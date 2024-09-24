#pragma once

#include "Camera.h"
#include "Base.h"
#include "Define.h"

class CDynamicCamera
	: public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CDynamicCamera();

public:
	static CDynamicCamera* Create
	(
		LPDIRECT3DDEVICE9 _pGraphicDev,
		const _vec3* _pEye,
		const _vec3* _pAt,
		const _vec3* _pUp,
		const _float& _fFov = D3DXToRadian(60.f),
		const _float& _fAspect = (float)WINCX / WINCY,
		const _float& _fNear = 0.1f,
		const _float& _fFar = 1000.f
	);

public:
	HRESULT	Ready_GameObject
	(
		const _vec3* _pEye,
		const _vec3* _pAt,
		const _vec3* _pUp,
		const _float& _fFov,
		const _float& _fAspect,
		const _float& _fNear,
		const _float& _fFar
	);
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject() {}

private:
	void Key_Input(const _float& _fTimeDelta);
	void Mouse_Move();
	void Mouse_Fix();

private:
	virtual void Free();

private:
	_bool m_bFix;
	_bool m_bCheck;
};