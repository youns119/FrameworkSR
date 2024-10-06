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
		LIFEBAR_LIFEBAR,
		LIFEBAR_LIFEBAR_A,
		LIFEBAR_LOGO,
		LIFEBAR_BATTERY,
		LIFEBAR_END,
	};

private:
	explicit CUIBossLifeBar(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIBossLifeBar();

public:
	static CUIBossLifeBar* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

private:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_END - 1];
};