#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIEmergency
	: public Engine::CUIUnit
{
private:
	explicit CUIEmergency(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIEmergency();

public:
	static CUIEmergency* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	_bool m_bActive;
	_float m_fTimerHP;
};