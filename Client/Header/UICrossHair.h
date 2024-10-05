#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUICrossHair
	: public Engine::CUIUnit
{
public:
	enum class UI_CROSSHAIR
	{
		CROSSHAIR_PISTOL,
		CROSSHAIR_RIFLE,
		CROSSHAIR_SNIPER,
		CROSSHAIR_SHOTGUN,
		CROSSHAIR_KATANA,
		CROSSHAIR_MINIGUN,
		CROSSHAIR_END,
	};

private:
	explicit CUICrossHair(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUICrossHair();

public:
	UI_CROSSHAIR Get_CrossHair() { return m_eCurrCrossHair; }

	void Set_CrossHair(UI_CROSSHAIR _eCurrCrossHair) { m_eCurrCrossHair = _eCurrCrossHair; }
	void Set_Free(_bool _bFree) { m_bFree = _bFree; }

public:
	static CUICrossHair* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	UI_CROSSHAIR m_eCurrCrossHair;
	_bool m_bFree;
};