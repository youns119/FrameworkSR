#pragma once

#include "UI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIOverlay
	: public Engine::CUI
{
public:
	enum class UI_OVERLAY
	{
		OVERLAY_BACK,
		OVERLAY_BASE,
		OVERLAY_END,
	};

private:
	explicit CUIOverlay(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIOverlay();

public:
	static CUIOverlay* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Component();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_END];
	Engine::CTransform* m_pTransformCom;
};