#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIIndicator;

class CUIFreeCam
	: public Engine::CUI
{
private:
	explicit CUIFreeCam(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIFreeCam();

public:
	static CUIFreeCam* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	CUIIndicator* m_pIndicator;
};