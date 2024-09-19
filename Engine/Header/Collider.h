#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider
	: public CComponent
{
private:
	explicit CCollider();
	explicit CCollider(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CCollider(const CCollider& _rhs);
	virtual ~CCollider();

public:
	_vec3 GetOffsetPos() { return m_vOffsetPos; }
	_vec3 GetFinalPos() { return m_vFinalPos; }
	_float GetRadius() { return m_fRadius; }
	_uint GetID() { return m_iID; }
	_bool GetShow() { return m_bShow; }

	void SetOffsetPos(const _vec3& _vPos) { m_vOffsetPos = _vPos; }
	void SetRadius(const _float& _fRadius) { m_fRadius = _fRadius; }
	void SetShow(_bool _bShow) { m_bShow = _bShow; }

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT	Ready_Collider();
	virtual void LateUpdate_Component();
	void Render_Collider();

public:
	void OnCollision(CCollider& _pOther);
	void OnCollisionEnter(CCollider& _pOther);
	void OnCollisionExit(CCollider& _pOther);

public:
	virtual CComponent* Clone();

private:
	virtual void Free();

private:
	_vec3 m_vOffsetPos;
	_vec3 m_vFinalPos;

	bool m_bActive;
	bool m_bShow;

	_float m_fRadius;

	_uint m_iCollided;
	_uint m_iID;
	static _uint g_iNextID;

	LPD3DXMESH m_pSphere;
};

END