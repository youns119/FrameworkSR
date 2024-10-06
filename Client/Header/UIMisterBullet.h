#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUISniperHP;
class CUIBossCount;

class CUIMisterBullet
	: public Engine::CUI
{
private:
	explicit CUIMisterBullet(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIMisterBullet();

public:
	static CUIMisterBullet* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

public:
	void Add_Count();

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	CUISniperHP* m_pUISniperHP;
	vector<CUIBossCount*> m_vecUIBossCount;

	_int m_iCount;
};