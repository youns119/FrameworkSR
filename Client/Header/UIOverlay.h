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
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_END];
	Engine::CTransform* m_pTransformCom;
};