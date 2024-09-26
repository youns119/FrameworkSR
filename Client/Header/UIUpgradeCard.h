#pragma once

#include "UI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIUpgradeCard
	: public Engine::CUI
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
	HRESULT Ready_UI(_vec3 _vPos);
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Component();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_CARD::CARD_END];
	Engine::CTransform* m_pTransformCom;

	UI_CARD m_eCurrCard;
};