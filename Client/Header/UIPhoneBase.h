#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CUIPhoneBase
	: public Engine::CUIUnit
{
public:
	enum class UI_PHONE
	{
		PHONE_RGBBACK1,
		PHONE_UPPER,
		PHONE_MODERATOR_NORMAL,
		PHONE_MODERATOR_HURT,
		PHONE_MODERATOR_HURTBAD,
		PHONE_BACK,
		PHONE_RGBBACK2,
		PHONE_RGBBACK3,
		PHONE_CARD,
		PHONE_DIGIT1,
		PHONE_DIGIT2,
		PHONE_INT,
		PHONE_END,
	};

	enum class MODERATOR_FACE
	{
		MODERATOR_NORMAL,
		MODERATOR_HURT,
		MODERATOR_HURTBAD,
		MODERATOR_END,
	};

private:
	explicit CUIPhoneBase(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIPhoneBase();

public:
	static CUIPhoneBase* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Set_Animation();
	void Update_Time();
	void Update_Color();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_PHONE::PHONE_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_PHONE::PHONE_END];
	Engine::CTransform* m_pTransformTexture;
	Engine::CAnimator* m_pAnimatorCom[2];

	IDirect3DTexture9* m_pTexture;
	IDirect3DSurface9* m_pTextureSurface;
	IDirect3DSurface9* m_pPreSurface;

	_int m_iDigit[2];
	_float m_fTick;
	_float m_fMaxHP;
	_float m_fTimerHP;
	D3DCOLOR m_tColor;
	MODERATOR_FACE m_eModeratorFace;
};