#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIShopBase;
class CUIShopCard;
class CUIShopBuzz;

class CUIShop
	: public Engine::CUI
{
private:
	explicit CUIShop(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIShop();

public:
	_bool Get_Finish() { return m_bFinished; }
	_float Get_FinishTime() { return m_fFinishTime; }
	_float Get_DownSpeed() { return m_fDownSpeed; }

	void Set_Finish(_bool _bFinished) { m_bFinished = _bFinished; }

public:
	static CUIShop* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	CUIShopBase* m_pUIShopBase;
	vector<CUIShopCard*> m_vecUIShopCard;
	CUIShopBuzz* m_pUIShopBuzz;

	_bool m_bFinished;
	_float m_fFinishTime;
	_float m_fDownSpeed;
};