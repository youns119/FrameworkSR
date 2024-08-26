#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject 
	: public CBase
{
protected :
	explicit CGameObject(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CGameObject(const CGameObject& _rhs);
	virtual ~CGameObject();

public :
	CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pComponentTag);

public :
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject() PURE;

private :
	CComponent* Find_Component(COMPONENTID _eID, const _tchar* _pComponentTag);

protected :
	virtual void Free();

protected :
	map<const _tchar*, CComponent*>	m_mapComponent[(UINT)COMPONENTID::ID_END];
	LPDIRECT3DDEVICE9 m_pGraphicDev;
};

END