#pragma once
#include "Camera.h"
#include "Base.h"
#include "Define.h"

class CMapCamera : public CCamera
{
private:
	explicit CMapCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapCamera();

public:
	HRESULT			Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& _fFov,
		const _float& _fAspect,
		const _float& _fNear,
		const _float& _fFar);

	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject();
	virtual			void			Render_GameObject() {}

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move();
	void		Mouse_Fix();

private:
	_bool			m_bFix;
	_bool			m_bCheck;

public:
	static CMapCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& _fFov = D3DXToRadian(60.f),
		const _float& _fAspect = (float)WINCX / WINCY,
		const _float& _fNear = 0.1f,
		const _float& _fFar = 1000.f);

private:
	virtual void	Free();
};

