#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIItem
	: public Engine::CUIUnit
{
public:
	enum class UI_ITEM
	{
		ITEM_OUTLINE,
		ITEM_INNER,
		ITEM_RGBBOX,
		ITEM_TEXTBOX,
		ITEM_EXECUTION,
		ITEM_MOUSE,
		ITEM_ICON,
		ITEM_END,
	};

private:
	explicit CUIItem(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIItem();

public:
	static CUIItem* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Update_Color(CTexture* _pTexture);

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_ITEM::ITEM_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_ITEM::ITEM_END];

	_bool m_bTick;
	_float m_fColor;
	_float m_fScaleRatio;
	_float m_fTick;
};