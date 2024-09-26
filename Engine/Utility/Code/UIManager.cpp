#include "UIManager.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CUIManager)

CUIManager::CUIManager()
{
	Clear_UI();

	for (_uint i = 0; i < (_uint)UITYPE::UI_END; ++i)
		m_LayerRender[i] = false;

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matOrtho, (_float)WINCX, (_float)WINCY, 0.f, 1.f);
}

CUIManager::~CUIManager()
{
	Free();
}

HRESULT CUIManager::Add_UI(CUI* _pUI)
{
	NULL_CHECK_RETURN(_pUI, E_FAIL);

	m_vecUI[(_uint)_pUI->Get_UIType()].push_back(_pUI);

	return S_OK;
}

_int CUIManager::Update_UI(const _float& _fTimeDelta)
{
	for (_uint i = 0; i < (_uint)UITYPE::UI_END; i++)
	{
		if (!m_LayerRender[i]) continue;

		for (auto& pUI : m_vecUI[i])
			pUI->Update_UI(_fTimeDelta);
	}

	return 0;
}

void CUIManager::LateUpdate_UI()
{
	for (_uint i = 0; i < (_uint)UITYPE::UI_END; i++)
	{
		if (!m_LayerRender[i]) continue;

		for (auto& pUI : m_vecUI[i])
		{
			pUI->LateUpdate_UI();

			if (pUI->Get_Render())
			{
				m_vecRender[i].push_back(pUI);
				pUI->AddRef();
			}
		}
	}
}

void CUIManager::Render_UI(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	_matrix matView, matProj;
	_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matOrtho);

	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (Engine::Get_ControllerID() == CONTROLLERID::CONTROL_PLAYER)
	{
		for (_uint i = 0; i < (_uint)UITYPE::UI_END; i++)
		{
			if (!m_LayerRender[i] || i == (_uint)UITYPE::UI_INDICATOR) continue;

			for (auto& pUI : m_vecRender[i])
				pUI->Render_UI();
		}
	}
	else if (m_LayerRender[(_uint)UITYPE::UI_INDICATOR])
		m_vecRender[(_uint)UITYPE::UI_INDICATOR][0]->Render_UI();

	_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	Clear_Render();
}

void CUIManager::Clear_Render()
{
	for (_uint i = 0; i < (_uint)UITYPE::UI_END; i++)
	{
		for_each(m_vecRender[i].begin(), m_vecRender[i].end(), CDeleteObj());
		m_vecRender[i].clear();
	}
}

void CUIManager::Clear_UI()
{
	for (_uint i = 0; i < (_uint)UITYPE::UI_END; i++)
	{
		for_each(m_vecUI[i].begin(), m_vecUI[i].end(), CDeleteObj());
		m_vecUI[i].clear();
	}

	Clear_Render();
}

void CUIManager::Free()
{
	Clear_UI();
}