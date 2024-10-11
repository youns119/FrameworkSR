#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIDownBase
	: public Engine::CUIUnit
{
public:
	enum class UI_DOWN
	{
		DOWN_BLACK,
		DOWN_SEC,
		DOWN_END,
	};

private:
	explicit CUIDownBase(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIDownBase();

public:
	static CUIDownBase* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Init(_int _iIndex);

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_DOWN::DOWN_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_DOWN::DOWN_END];

	_bool m_bTick;
	_float m_fTimeDelay;
	_float m_fTimer;
	_float m_fTick;
	_float m_fScaleRatio;
	_float m_fAlpha;
};