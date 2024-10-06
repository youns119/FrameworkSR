#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIBossCount
	: public Engine::CUIUnit
{
private:
	explicit CUIBossCount(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIBossCount();

public:
	void Set_Count(_int _iCount) { m_iCount = _iCount; }

public:
	static CUIBossCount* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _int _iIndex);

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
	_int m_iCount;
	_float m_fDist;
};