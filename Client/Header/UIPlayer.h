#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIReload;
class CUIItem;

class CUIPlayer
	: public Engine::CUI
{
private:
	explicit CUIPlayer(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIPlayer();

public:
	static CUIPlayer* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	CUIReload* m_pUIReload;
	CUIItem* m_pUIItem;
};