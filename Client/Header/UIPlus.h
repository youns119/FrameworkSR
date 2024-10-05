#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIPlusCenter;
class CUIPlusLeft;

class CUIPlus
	: public Engine::CUI
{
public:
	enum class UI_PLUS
	{
		PLUS_AIR,
		PLUS_SCOLD,
		PLUS_BAN,
		PLUS_EXECUTE,
		PLUS_GOODBYE,
		PLUS_GRINDER,
		PLUS_HEADSHOT,
		PLUS_NUTSHOT,
		PLUS_SAYONARA,
		PLUS_THUNDER,
		PLUS_SHATTER,
		PLUS_SHOCK,
		PLUS_HIT,
		PLUS_END,
	};

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
	void Init(_vec3 _vPos, UI_PLUS _ePlusType);

private:
	virtual void Free();

private:
	CUIPlusCenter* m_pUIPlusCenter;
	CUIPlusLeft* m_pUIPlusLeft;

	UI_PLUS m_ePlusType;
	_float m_fLifeTime;
	_float m_fElapsed;
	_int m_iBlink;
};