#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUISniperHP
	: public Engine::CUIUnit
{
public:
	enum class UI_SNIPERHP
	{
		SNIPERHP_BOX1,
		SNIPERHP_BOX2,
		SNIPERHP_BAR,
		SNIPERHP_DIGIT1,
		SNIPERHP_DIGIT2,
		SNIPERHP_DIGIT3,
		SNIPERHP_END,
	};

private:
	explicit CUISniperHP(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUISniperHP();

public:
	void Set_HP(_float _fHP) { m_fHP = _fHP; }

public:
	static CUISniperHP* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	Engine::CTexture* m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_END];

	_float m_fHP;
	_float m_fScaleRatio;
};