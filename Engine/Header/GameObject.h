#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class CCollider;

class ENGINE_DLL CGameObject 
	: public CBase
{
protected :
	explicit CGameObject(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CGameObject(const CGameObject& _rhs);
	virtual ~CGameObject();

public :
	CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pComponentTag);
	_float Get_ViewZ() const { return m_fViewZ; }
	virtual void Set_IsRender(const _bool& _bool) { m_bIsRender = _bool; } //For Trigger(Tile, Monster)
	_bool Get_IsRender() { return m_bIsRender; } //For Trigger(Tile, Monster)


public :
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject() PURE;

private :
	CComponent* Find_Component(COMPONENTID _eID, const _tchar* _pComponentTag);

public :
	void Compute_ViewZ(const _vec3* _pPos);

public :
	virtual void OnCollision(CCollider& _pOther) {};
	virtual void OnCollisionEnter(CCollider& _pOther) {};
	virtual void OnCollisionExit(CCollider& _pOther) {};

protected :
	virtual void Free();

protected :
	map<const _tchar*, CComponent*>	m_mapComponent[(_uint)COMPONENTID::ID_END];
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	_float m_fViewZ;
	_bool m_bIsRender; //For Trigger
};

END