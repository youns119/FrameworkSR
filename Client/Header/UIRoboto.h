#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIBossLifeBar;

class CUIRoboto
	: public Engine::CUI
{
private:
	explicit CUIRoboto(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIRoboto();

public:
	static CUIRoboto* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

public:
	void Boss_Hit();

private:
	virtual void Free();

private:
	CUIBossLifeBar* m_pUIBossLifeBar;
};