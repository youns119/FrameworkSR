#pragma once

#include "UI.h"

BEGIN(Engine)

class CUIUnit;

END

class CUIActionBase;

class CUIAction
	: public Engine::CUI
{
public:
	enum class ACTION_TYPE
	{
		TYPE_KNIFE,
		TYPE_DRINK,
		TYPE_END,
	};

private:
	explicit CUIAction(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIAction();

public:
	static CUIAction* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_UI();
	virtual	_int Update_UI(const _float& _fTimeDelta);
	virtual	void LateUpdate_UI();
	virtual	void Render_UI();

private:
	HRESULT Add_Unit();

public:
	ACTION_TYPE Get_ActionType();

	void Set_ActionType(ACTION_TYPE _eActionType);

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	CUIActionBase* m_pUIActionBase;

	_float m_fDuration;
};