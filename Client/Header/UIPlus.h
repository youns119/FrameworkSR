#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIPlus
	: public Engine::CUI
{
private:
	explicit CUIPlus(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIPlus();

public:
	static CUIPlus* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

public:
	void Set_Pos(_vec3 _vPos);
	void Set_Index(_int _iIndex);

private:
	virtual void Free();

private:
	Engine::CUIUnit* m_pUIPlusCenter;
	Engine::CUIUnit* m_pUIPlusLeft;

	_float m_fLifeTime;
	_float m_fElapsed;
	_int m_iBlink;
};