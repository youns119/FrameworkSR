#pragma once

#include "Base.h"
#include "UIUnit.h"

BEGIN(Engine)

class ENGINE_DLL CUI
	: public CBase
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CUI(const CUI& _rhs);
	virtual ~CUI();

public:
	UITYPE Get_UIType() const { return m_eUIType; }
	_float Get_ViewZ() const { return m_fViewZ; }
	_float Get_Render() const { return m_bRender; }

	void Set_Render(_bool _bRender) { m_bRender = _bRender; }

public:
	virtual _int Update_UI(const _float& _fTimeDelta);
	virtual void LateUpdate_UI();
	virtual void Render_UI();

public:
	virtual void Reset();

protected:
	virtual void Free();

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	vector<CUIUnit*> m_vecUIUnit;

	UITYPE m_eUIType;
	bool m_bRender;
	float m_fViewZ;
};

END