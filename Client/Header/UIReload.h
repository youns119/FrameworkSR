#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIReload
	: public Engine::CUIUnit
{
public:
	enum class UI_RELOAD
	{
		RELOAD_REDBOX,
		RELOAD_BLACKBOX,
		RELOAD_RGBBOX,
		RELOAD_R,
		RELOAD_RELOAD,
		RELOAD_END,
	};

private:
	explicit CUIReload(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIReload();

public:
	static CUIReload* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Update_Color();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_RELOAD::RELOAD_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_RELOAD::RELOAD_END];

	_bool m_bReload;
};