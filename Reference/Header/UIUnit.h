#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class CUI;

class ENGINE_DLL CUIUnit
	: public CBase
{
protected:
	explicit CUIUnit(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CUIUnit(const CUIUnit& _rhs);
	virtual ~CUIUnit();

public:
	CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pComponentTag);
	_bool Get_Render() const { return m_bRender; }
	_float Get_ViewZ() const { return m_fViewZ; }

	void Set_Render(_bool _bRender) { m_bRender = _bRender; }
	void Set_OwnerUI(CUI* _pUI) { m_pOwnerUI = _pUI; }

public:
	virtual _int Update_Unit(const _float& _fTimeDelta);
	virtual void LateUpdate_Unit();
	virtual void Render_Unit() {};

public:
	virtual void On_Click() {};
	virtual void On_Hover() {};
	virtual void Reset() {};

private:
	CComponent* Find_Component(COMPONENTID _eID, const _tchar* _pComponentTag);

protected:
	virtual void Free();

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	map<const _tchar*, CComponent*>	m_mapComponent[(_uint)COMPONENTID::ID_END];

	_bool m_bRender;
	_float m_fViewZ;

	CUI* m_pOwnerUI;
};

END