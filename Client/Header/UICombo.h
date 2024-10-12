#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIComboBase;

class CUICombo
	: public Engine::CUI
{
private:
	explicit CUICombo(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUICombo();

public:
	static CUICombo* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

public:
	_int Get_Combo();

	void Combo_Up();

private:
	virtual void Free();

private:
	CUIComboBase* m_pUIComboBase;
};