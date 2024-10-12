#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIBossLifeBar
	: public Engine::CUIUnit
{
public:
	enum class UI_LIFEBAR
	{
		LIFEBAR_LIFEBAR_P,
		LIFEBAR_LIFEBAR_B,
		LIFEBAR_LIFEBAR,
		LIFEBAR_LIFEBAR_A,
		LIFEBAR_LOGO,
		LIFEBAR_BATTERY,
		LIFEBAR_TEXT,
		LIFEBAR_BAR,
		LIFEBAR_END,
	};

private:
	explicit CUIBossLifeBar(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIBossLifeBar();

public:
	void Boss_Hit()
	{
		m_bHit = true;
		m_fHitTime = 0.f;
		m_fAlpha = 1.f;
	}

public:
	static CUIBossLifeBar* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Hit_Effect();

private:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_END - 1];
	Engine::CTexture* m_pBarTextureCom[18];
	Engine::CTransform* m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_END];
	Engine::CTransform* m_pBarTransformCom[18];
	Engine::CTransform* m_pTransformTexture;

	IDirect3DTexture9* m_pTexture;
	IDirect3DSurface9* m_pTextureSurface;
	IDirect3DSurface9* m_pPreSurface;

	_bool m_bHit;
	_int m_iCount;
	_float m_fMaxHP;
	_float m_fCurrHP;
	_float m_fHitTime;
	_float m_fAlpha;
};