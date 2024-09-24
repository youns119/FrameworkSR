#pragma once

#include "UI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUICrossHair
	: public Engine::CUI
{
public:
	enum class UI_CROSSHAIR
	{
		CROSSHAIR_PISTOL,
		CROSSHAIR_RIFLE,
		CROSSHAIR_SNIPER,
		CROSSHAIR_SHOTGUN,
		CROSSHAIR_END,
	};

private:
	explicit CUICrossHair(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUICrossHair();

public:
	static CUICrossHair* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_CROSSHAIR::CROSSHAIR_END];

	UI_CROSSHAIR m_eCurrCrossHair;
};