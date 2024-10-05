#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIOverlay;
class CUIUpgradeCard;

class CUIInventory
	: public Engine::CUI
{
private:
	explicit CUIInventory(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIInventory();

public:
	static CUIInventory* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

public:
	void Update_Inventory(_uint _iIndex);

private:
	virtual void Free();

private:
	CUIOverlay* m_pOverlay;
	vector<CUIUpgradeCard*> m_vecUpgradeCard;

	vector<_int> m_vecInventory;
};