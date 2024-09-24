#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CUI
	: public CBase
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CUI(const CUI& _rhs);
	virtual ~CUI();

public:
	CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pComponentTag);
	UITYPE Get_UIType() { return m_eUIType; }
	bool Get_Active() { return m_bActive; }

	void Set_Active(bool _bActive) { m_bActive = _bActive; }

public:
	virtual _int Update_UI(const _float& _fTimeDelta);
	virtual void LateUpdate_UI();
	virtual void Render_UI() PURE;

private:
	CComponent* Find_Component(COMPONENTID _eID, const _tchar* _pComponentTag);

protected:
	virtual void Free();

protected:
	map<const _tchar*, CComponent*>	m_mapComponent[(_uint)COMPONENTID::ID_END];
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	UITYPE m_eUIType;
	bool m_bActive;
};

END