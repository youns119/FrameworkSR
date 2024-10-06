#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIBossLifeBar;
class CUIBossBar;
class CUIBossName;

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
	virtual void Reset();

private:
	virtual void Free();

private:
	CUIBossLifeBar* m_pUIBossLifeBar;
	vector<CUIBossBar*> m_vecUIBossBar;
	CUIBossName* m_pUIBossName;

	_float m_fMaxHP;
	_float m_fCurrHP;
};