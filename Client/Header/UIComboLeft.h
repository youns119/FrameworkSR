#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIComboLeft
	: public Engine::CUIUnit
{
public:
	enum class UI_COMBOLEFT
	{
		COMBOLEFT_BACK,
		COMBOLEFT_X,
		COMBOLEFT_DIGIT1,
		COMBOLEFT_DIGIT2,
		COMBOLEFT_END,
	};

private:
	explicit CUIComboLeft(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIComboLeft();

public:
	static CUIComboLeft* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	Engine::CTexture* m_pTextureCom[(_uint)UI_COMBOLEFT::COMBOLEFT_END];
	Engine::CTransform* m_pTransformCom[2];

	_float m_fScaleRatio;
	_int m_iDigit;
};