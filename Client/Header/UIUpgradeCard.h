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
	enum class UI_CARD
	{
		CARD_BLANK,
		CARD_WEAPON,
		CARD_KATANA,
		CARD_END,
	};

private:
	explicit CUIUpgradeCard(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIUpgradeCard();

public:
	static CUIUpgradeCard* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vPos);

public:
	HRESULT Ready_Unit(_vec3 _vPos);
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_CARD::CARD_END];

	UI_CARD m_eCurrCard;
};