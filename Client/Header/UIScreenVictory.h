#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIScreenVictory
	: public Engine::CUIUnit
{
public:
	enum class UI_SCREENVICTORY
	{
		SCREENVICTORY_ICON,
		SCREENVICTORY_LIVE,
		SCREENVICTORY_BALLOON,
		SCREENVICTORY_BACKGROUND,
		SCREENVICTORY_VICTORY,
		SCREENVICTORY_OMEDETO,
		SCREENVICTORY_BARBACK,
		SCREENVICTORY_BAR,
		SCREENVICTORY_FLOOR,
		SCREENVICTORY_DOT,
		SCREENVICTORY_DIGIT1,
		SCREENVICTORY_DIGIT2,
		SCREENVICTORY_DIGIT3,
		SCREENVICTORY_DIGIT4,
		SCREENVICTORY_DIGIT5,
		SCREENVICTORY_SEC,
		SCREENVICTORY_END,
	};

private:
	explicit CUIScreenVictory(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIScreenVictory();

public:
	void Set_FloorTime(_float _fFloorTime)
	{
		m_fFloorTime = _fFloorTime;
		Update_FloorTime();
	}

public:
	static CUIScreenVictory* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

private:
	void Update_Color(CTexture* _pTexture);
	void Update_FloorTime();

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_SCREENVICTORY::SCREENVICTORY_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_SCREENVICTORY::SCREENVICTORY_END];
	Engine::CTransform* m_pTextureTransformCom;

	_bool m_bTextureRendered;
	_float m_fScaleRatio[3];
	_float m_fSpeed;
	_float m_fDelayTime;
	_float m_fAlpha[2];
	_float m_fFloorTime;
	_int m_iDigit[5];

	IDirect3DTexture9* m_pTexture;
	IDirect3DSurface9* m_pTextureSurface;
	IDirect3DSurface9* m_pPreSurface;
};