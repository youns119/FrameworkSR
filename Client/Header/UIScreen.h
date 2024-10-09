#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIScreenBase;
class CUIScreenVictory;

class CUIScreen
	: public Engine::CUI
{
private:
	explicit CUIScreen(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIScreen();

public:
	static CUIScreen* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

public:
	void Set_FloorTime(_float _fFloorTime);
	void Set_Return(_bool _bReturn);

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	CUIScreenBase* m_pUIScreenBase;
	CUIScreenVictory* m_pUIScreenVictory;

	_bool m_bShop;
	_float m_fShopTime;
};