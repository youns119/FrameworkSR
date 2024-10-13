#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CUIScreenBase
	: public Engine::CUIUnit
{
public:
	enum class UI_SCREENBASE
	{
		SCREENBASE_WIDE,
		SCREENBASE_FRAME1,
		SCREENBASE_FRAME2,
		SCREENBASE_SCREEN,
		SCREENBASE_FINE,
		SCREENBASE_LINE,
		SCREENBASE_LOGO,
		SCREENBASE_CHAT,
		SCREENBASE_FOLLOW,
		SCREENBASE_FLOOR,
		SCREENBASE_STATIC,
		SCREENBASE_STREAMER_FACE1,
		SCREENBASE_STREAMER_FACE2,
		SCREENBASE_STREAMER_FACE3,
		SCREENBASE_STREAMER_FACE4,
		SCREENBASE_STREAMER_HAIR1,
		SCREENBASE_STREAMER_HAIR2,
		SCREENBASE_STREAMER_HAIR3,
		SCREENBASE_STREAMER_HAIR4,
		SCREENBASE_INT,
		SCREENBASE_PEACE,
		SCREENBASE_CLICKMUTE,
		SCREENBASE_STREAMERNUMBER,
		SCREENBASE_STREAMERNAME,
		SCREENBASE_END,
	};

private:
	explicit CUIScreenBase(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIScreenBase();

public:
	_bool Get_BackToNormal()
	{
		if (m_bBackToNormal[0] && m_bBackToNormal[1])
			return true;
		else return false;
	}
	_int Get_Floor() { return m_iFloor; }

	void Set_Return(_bool _bReturn) { m_bReturn = _bReturn; }
	void Set_Floor(_int _iFloor) { m_iFloor = _iFloor; }

public:
	static CUIScreenBase* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Set_Animation();
	void Update_Color();

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_END];
	Engine::CAnimator* m_pAnimatorCom[5];

	IDirect3DTexture9* m_pTexture;
	IDirect3DTexture9* m_pWideTexture;
	IDirect3DSurface9* m_pWideSurface;
	IDirect3DSurface9* m_pPreSurface;

	_bool m_bReturn;
	_bool m_bBackToNormal[2];
	_int m_iFloor;
	_int m_iPhase;
	_float m_fBuzzTime;
};