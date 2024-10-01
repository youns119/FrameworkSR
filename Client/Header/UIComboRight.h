#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIComboRight
	: public Engine::CUIUnit
{
public:
	enum class UI_COMBORIGHT
	{
		COMBORIGHT_BACK,
		COMBORIGHT_COMBO,
		COMBORIGHT_SEC,
		COMBORIGHT_END,
	};

private:
	explicit CUIComboRight(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIComboRight();

public:
	static CUIComboRight* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_COMBORIGHT::COMBORIGHT_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_COMBORIGHT::COMBORIGHT_END];

	_float m_fScaleRatio;
};