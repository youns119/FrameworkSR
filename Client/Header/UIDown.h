#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIDownBase;

class CUIDown
	: public Engine::CUI
{
private:
	explicit CUIDown(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIDown();

public:
	static CUIDown* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

public:
	void Init(_int _iIndex);

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	CUIDownBase* m_pUIDownBase;

	_float m_fDuration;
};