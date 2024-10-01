#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIIndicator
	: public Engine::CUIUnit
{
private:
	explicit CUIIndicator(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIIndicator();

public:
	void Set_Pos(_vec3 _vPos) { m_vPos = _vPos; }

public:
	static CUIIndicator* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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

	_vec3 m_vPos;
};