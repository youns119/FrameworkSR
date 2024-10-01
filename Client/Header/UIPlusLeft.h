#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIPlusLeft
	: public Engine::CUIUnit
{
public:
	enum class UI_PLUSLEFT
	{
		PLUSLEFT_BACK_SEC,
		PLUSLEFT_SEC,
		PLUSLEFT_BACK_TEXT,
		PLUSLEFT_TEXT,
		PLUSLEFT_END,
	};

private:
	explicit CUIPlusLeft(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIPlusLeft();

public:
	void Set_Index(_int _iIndex)
	{
		m_iIndex = _iIndex;
		m_fSpeed += (m_iIndex * 0.5f);
	}

public:
	static CUIPlusLeft* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Set_Text(_int _iIndex);

private:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_END];

	_float m_fScaleRatio[2];
	_float m_fAlphaRatio[2];
	_float m_fSpeed;
	_float m_fScaleSpeed;
	_float m_fTextTime;
	_float m_fElapsed;
	_int m_iTextIndex;
	_int m_iSecIndex;
	_int m_iIndex;
};