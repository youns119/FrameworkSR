#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUICrossHair;

class CUINormal
	: public Engine::CUI
{
private:
	explicit CUINormal(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUINormal();

public:
	static CUINormal* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	CUICrossHair* m_pCrossHair;
};