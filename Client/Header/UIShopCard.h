#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CUIShopCard
	: public Engine::CUIUnit
{
public:
	enum class UI_SHOPCARD
	{
		SHOPCARD_BASE,
		SHOPCARD_BOARDER_A,
		SHOPCARD_PURCHASE,
		SHOPCARD_BOARDER,
		SHOPCARD_END,
	};

	enum class UI_UPGRADE
	{
		UPGRADE_MASCULINITY,
		UPGRADE_SLOWTIME,
		UPGRADE_SLOWBULLET,
		UPGRADE_TEMP1,
		UPGRADE_TEMP2,
		UPGRADE_TEMP3,
		UPGRADE_BLANK,
		UPGRADE_END,
	};

private:
	explicit CUIShopCard(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIShopCard();

public:
	_bool Get_Selected() { return m_bSelected; }

	void Set_Upgrade(UI_UPGRADE _eUIUpgrade) { m_eUIUpgrade = _eUIUpgrade; }
	void Set_Start(_bool _bStart) { m_bStart = _bStart; }
	void Set_Index(_int _iIndex) { m_iIndex = _iIndex; }

public:
	static CUIShopCard* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _int _iIndex);

public:
	HRESULT Ready_Unit(_int _iIndex);
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

private:
	void Set_Animation();
	void Update_Color();

public:
	virtual void On_Click();
	virtual void On_Hover();

private:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_END - 1];
	Engine::CAnimator* m_pAnimatorCom;

	UI_UPGRADE m_eUIUpgrade;
	_bool m_bStart;
	_bool m_bHover;
	_bool m_bClick;
	_bool m_bSelected;
	_int m_iIndex;

	RECT m_tRect;
};