#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIUpgradeCard
	: public Engine::CUIUnit
{
public:
	enum class UI_UPGRADE
	{
		UPGRADE_CARD,
		UPGRADE_BOARDER,
		UPGRADE_END,
	};

	enum class UI_CARD
	{
		CARD_MASCULINITY,
		CARD_SLOWTIME,
		CARD_SLOWBULLET,
		CARD_TEMP1,
		CARD_TEMP2,
		CARD_TEMP3,
		CARD_BLANK,
		CARD_END,
	};

private:
	explicit CUIUpgradeCard(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIUpgradeCard();

public:
	UI_CARD Get_CurrCard() { return m_eCurrCard; }

	void Set_CurrCard(UI_CARD _eCurrCard) { m_eCurrCard = _eCurrCard; }

public:
	static CUIUpgradeCard* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vPos);

public:
	HRESULT Ready_Unit(_vec3 _vPos);
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	virtual void On_Hover();

private:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_UPGRADE::UPGRADE_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_END];

	UI_CARD m_eCurrCard;
	_bool m_bHover;

	RECT m_tRect;
};