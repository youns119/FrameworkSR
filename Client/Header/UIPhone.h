#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIPhoneBase;
class CUIPhoneBoss;

class CUIPhone
	: public Engine::CUI
{
private:
	explicit CUIPhone(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIPhone();

public:
	static CUIPhone* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

private:
	virtual void Free();

private:
	CUIPhoneBase* m_pUIPhoneBase;
	CUIPhoneBoss* m_pUIPhoneBoss;
};