#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIEndingBase;

class CUIEnding
	: public Engine::CUI
{
private:
	explicit CUIEnding(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIEnding();

public:
	static CUIEnding* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	CUIEndingBase* m_pUIEndingBase;
};