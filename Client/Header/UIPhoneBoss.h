#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CUIPhoneBoss
	: public Engine::CUIUnit
{
public:
	enum class UI_BOSSPHONE
	{
		BOSSPHONE_BACK1,
		BOSSPHONE_UPPER,
		BOSSPHONE_BAR,
		BOSSPHONE_MODERATOR_NORMAL,
		BOSSPHONE_MODERATOR_HURT,
		BOSSPHONE_MODERATOR_HURTBAD,
		BOSSPHONE_BACK2,
		BOSSPHONE_BACK3,
		BOSSPHONE_CARD,
		BOSSPHONE_DIGIT1,
		BOSSPHONE_DIGIT2,
		BOSSPHONE_PERCENT,
		BOSSPHONE_INT,
		BOSSPHONE_END,
	};

	enum class MODERATOR_FACE
	{
		MODERATOR_NORMAL,
		MODERATOR_HURT,
		MODERATOR_HURTBAD,
		MODERATOR_END,
	};

private:
	explicit CUIPhoneBoss(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIPhoneBoss();

public:
	static CUIPhoneBoss* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_BOSSPHONE::BOSSPHONE_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_BOSSPHONE::BOSSPHONE_END];
	Engine::CTransform* m_pTransformTexture;
	Engine::CAnimator* m_pAnimatorCom[2];

	IDirect3DTexture9* m_pTexture;
	IDirect3DSurface9* m_pTextureSurface;
	IDirect3DSurface9* m_pPreSurface;

	_int m_iDigit[2];
	_float m_fMaxHP;
	_float m_fHP;

	MODERATOR_FACE m_eModeratorFace;
};