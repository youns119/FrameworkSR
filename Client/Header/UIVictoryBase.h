#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIVictoryBase
	: public Engine::CUIUnit
{
public:
	enum class UI_VICTORY
	{
		VICTORY_ICON,
		VICTORY_LIVE,
		VICTORY_BALLOON,
		VICTORY_BACKGROUND,
		VICTORY_VICTORY,
		VICTORY_OMEDETO,
		VICTORY_BARBACK,
		VICTORY_BAR,
		VICTORY_FLOOR,
		VICTORY_DOT,
		VICTORY_DIGIT1,
		VICTORY_DIGIT2,
		VICTORY_DIGIT3,
		VICTORY_DIGIT4,
		VICTORY_DIGIT5,
		VICTORY_SEC,
		VICTORY_END,
	};

private:
	explicit CUIVictoryBase(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIVictoryBase();

public:
	void Set_FloorTime(_float _fFloorTime)
	{
		m_fFloorTime = _fFloorTime;
		Update_FloorTime();
	}

public:
	static CUIVictoryBase* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	Engine::CTexture* m_pTextureCom[(_uint)UI_VICTORY::VICTORY_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_VICTORY::VICTORY_END];
	Engine::CTransform* m_pTextureTransformCom;

	_bool m_bTextureRendered;
	_bool m_bText;
	_float m_fScaleRatio[3];
	_float m_fSpeed;
	_float m_fDelayTime;
	_float m_fAlpha[2];
	_float m_fFloorTime;
	_float m_fWordTime;
	_int m_iDigit[5];
	_int m_iFloor;
	_int m_iWordCount;

	_vec2 m_vTextPos;

	IDirect3DTexture9* m_pTexture;
	IDirect3DSurface9* m_pTextureSurface;
	IDirect3DSurface9* m_pPreSurface;

	_tchar m_szComment1[128];
	_tchar m_szComment2[128];
	_tchar m_szComment3[128];
};