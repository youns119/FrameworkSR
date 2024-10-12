#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIComboBase
	: public Engine::CUIUnit
{
public:
	enum class UI_COMBO
	{
		COMBO_RIGHTBACK,
		COMBO_COMBO,
		COMBO_PLUS,
		COMBO_RIGHTDIGIT1,
		COMBO_RIGHTDIGIT2,
		COMBO_SEC,
		COMBO_LEFTBACK,
		COMBO_X,
		COMBO_LEFTDIGIT1,
		COMBO_LEFTDIGIT2,
		COMBO_END,
	};

private:
	explicit CUIComboBase(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIComboBase();

public:
	_int Get_Combo()
	{
		if (m_iCombo > 1) return m_iCombo;
		else return 0;
	}

	void Combo_Up()
	{
		m_iCombo++;
		m_bTick = false;
		m_fTickTime = 0.f;
		m_fDuration = 0.f;
	}

public:
	static CUIComboBase* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Update_Combo();

private:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_COMBO::COMBO_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_COMBO::COMBO_END];
	Engine::CTransform* m_pTransformTexture;

	IDirect3DTexture9* m_pTexture;
	IDirect3DSurface9* m_pTextureSurface;
	IDirect3DSurface9* m_pPreSurface;

	_bool m_bTick;
	_int m_iCombo;
	_int m_iRightDigit[2];
	_int m_iLeftDigit[2];
	_float m_fScaleRatio[2];
	_float m_fTimeDelay;
	_float m_fTickTime;
	_float m_fDuration;
};