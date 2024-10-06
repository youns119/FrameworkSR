#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIBossBar
	: public Engine::CUIUnit
{
private:
	explicit CUIBossBar(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIBossBar();

public:
	static CUIBossBar* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _int _iIndex);

public:
	HRESULT Ready_Unit(_int _iIndex);
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
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	_int m_iIndex;
};