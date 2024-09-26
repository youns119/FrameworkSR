#pragma once

#include "Base.h"
#include "UI.h"

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
	vector<CUI*>* Get_UIGroup(UITYPE _eUIType) { return &m_vecUI[(_uint)_eUIType]; }
	_bool Get_UILayerRender(UITYPE _eUIType) const { return m_LayerRender[(_uint)_eUIType]; }

	void Set_UILayerRender(UITYPE _eUIType, _bool _bRender) { m_LayerRender[(_uint)_eUIType] = _bRender; }

public:
	void Toggle_UILayer(UITYPE _eUIType) { m_LayerRender[(_uint)_eUIType] = !m_LayerRender[(_uint)_eUIType]; }

private:
	void Clear_Render();
	void Clear_UI();

private:
	virtual void Free();

private:
	vector<CUI*> m_vecUI[(_uint)UITYPE::UI_END];
	vector<CUI*> m_vecRender[(_uint)UITYPE::UI_END];

	_bool m_LayerRender[(_uint)UITYPE::UI_END];

	_matrix m_matView, m_matOrtho;
};

END