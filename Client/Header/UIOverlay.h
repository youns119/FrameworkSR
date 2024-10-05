#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIOverlay
	: public Engine::CUIUnit
{
public:
	enum class UI_OVERLAY
	{
		OVERLAY_BACK,
		OVERLAY_BASE,
		OVERLAY_ICON,
		OVERLAY_LIVESTREAM,
		OVERLAY_MODERATOR,
		OVERLAY_MODERATOR_A,
		OVERLAY_END,
	};

private:
	explicit CUIOverlay(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIOverlay();

public:
	static CUIOverlay* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

private:
	void Update_Color(CTexture* _pTexture);

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_END];
	Engine::CTransform* m_pTransformCom[5];
};