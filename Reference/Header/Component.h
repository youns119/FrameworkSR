#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CComponent
	: public CBase
{
protected :
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CComponent(const CComponent& _rhs);
	virtual ~CComponent();

public :
	CGameObject* GetOwner() { return m_pOwner; }

	void SetOwner(CGameObject& _pOwner) { m_pOwner = &_pOwner; }

public :
	virtual _int Update_Component(const _float& _fTimeDelta) { return 0; }
	virtual void LateUpdate_Component() {}

public :
	virtual CComponent* Clone() PURE;

protected :
	virtual void Free();

protected :
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// Owner 변수 추가
	CGameObject* m_pOwner;
	_bool m_bClone;
};

END