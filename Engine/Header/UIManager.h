#pragma once

#include "UI.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CUIManager
	: public CBase
{
	DECLARE_SINGLETON(CUIManager)

private:
	explicit CUIManager();
	virtual ~CUIManager();

public:
	HRESULT	Add_UI(CUI* _pUI);
	_int Update_UI(const _float& _fTimeDelta);
	void LateUpdate_UI();
	void Render_UI(LPDIRECT3DDEVICE9& _pGraphicDev);

public:
	list<CUI*>* Get_UIList(UITYPE _eUIType) { return &m_listUI[(_uint)_eUIType]; }

private:
	void Clear_Render();
	void Clear_UI();

private:
	virtual void Free();

private:
	list<CUI*> m_listUI[(_uint)UITYPE::UI_END];
	list<CUI*> m_listRender[(_uint)UITYPE::UI_END];

	_matrix m_matView, m_matOrtho;
};

END