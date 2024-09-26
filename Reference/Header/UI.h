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
	UITYPE Get_UIType() const { return m_eUIType; }
	_bool Get_Render() const { return m_bRender; }
	CGameObject* Get_GameObject() { return m_pGameObject; }

	void Set_Render(_bool _bRender) { m_bRender = _bRender; }
	void Set_GameObject(CGameObject* _pGameObject) { m_pGameObject = _pGameObject; }

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
	bool m_bRender;

	CGameObject* m_pGameObject;
};

END