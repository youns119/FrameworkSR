#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIPlusCenter
	: public Engine::CUIUnit
{
public:
	enum class UI_PLUSCENTER
	{
		PLUSCENTER_BACK,
		PLUSCENTER_SEC,
		PLUSCENTER_END,
	};

private:
	explicit CUIPlusCenter(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIPlusCenter();

public:
	void Set_Pos(_vec3 _vPos) { m_vPos = _vPos; }

public:
	static CUIPlusCenter* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Set_Sec(_int _iIndex);

private:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_PLUSCENTER::PLUSCENTER_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_PLUSCENTER::PLUSCENTER_END];

	_vec3 m_vPos;
	_float m_fScaleRatio;
	_float m_fAlphaRatio;
	_int m_iSecIndex;
};