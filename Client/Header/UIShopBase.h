#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CUIShopBase
	: public Engine::CUIUnit
{
public:
	enum class UI_SHOPBASE
	{
		SHOPBASE_START,
		SHOPBASE_LOAD,
		SHOPBASE_BASE,
		SHOPBASE_END,
	};

private:
	explicit CUIShopBase(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIShopBase();

public:
	static CUIShopBase* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

private:
	void Set_Animation();

private:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_END];
	Engine::CTransform* m_pTransformCom;
	Engine::CAnimator* m_pAnimatorCom;

	_float m_fScaleRatio;
};