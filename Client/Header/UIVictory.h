#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIVictoryBase;

class CUIVictory
	: public Engine::CUI
{
private:
	explicit CUIVictory(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIVictory();

public:
	static CUIVictory* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

public:
	void Set_FloorTime(_float _fFloorTime);

private:
	virtual void Free();

private:
	CUIVictoryBase* m_pUIVictoryBase;
};