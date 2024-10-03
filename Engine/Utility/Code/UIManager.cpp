#include "UIManager.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CUIManager)

CUIManager::CUIManager()
{
	Clear_UI();

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

	m_queueUI[(_uint)_pUI->Get_UIType()].push(_pUI);

	return S_OK;
}

_int CUIManager::Update_UI(const _float& _fTimeDelta)
{
	for (_uint i = 0; i < (_uint)UITYPE::UI_END; ++i)
		for (auto& pUI : m_listUI[i])
			pUI->Update_UI(_fTimeDelta);

	return 0;
}

void CUIManager::LateUpdate_UI()
{
	for (_uint i = 0; i < (_uint)UITYPE::UI_END; ++i)
	{
		for (auto pUI = m_listUI[i].begin(); pUI != m_listUI[i].end();)
		{
			if (!(*pUI)->Get_Render())
			{
				(*pUI)->Reset();
				m_queueUI[(_uint)(*pUI)->Get_UIType()].push(*pUI);
				pUI = m_listUI[i].erase(pUI);
			}
			else
			{
				(*pUI)->LateUpdate_UI();
				pUI++;
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
		for (_uint i = 0; i < (_uint)UITYPE::UI_END; ++i)
			for (auto& pUI : m_listUI[i])
			{
				if (pUI->Get_UIType() == UITYPE::UI_FREECAM) continue;
				else pUI->Render_UI();
			}
	}
	else
		for (auto& pUI : m_listUI[(_uint)UITYPE::UI_FREECAM])
			pUI->Render_UI();

	_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CUI* CUIManager::Activate_UI(UITYPE _eUIType)
{
	CUI* pUI = m_queueUI[(_uint)_eUIType].front();
	pUI->Set_Render(true);

	m_listUI[(_uint)_eUIType].push_back(pUI);
	m_queueUI[(_uint)_eUIType].pop();

	return pUI;
}

void CUIManager::Deactivate_UI(UITYPE _eUIType)
{
	for (auto pUI = m_listUI[(_uint)_eUIType].begin(); pUI != m_listUI[(_uint)_eUIType].end();)
	{
		(*pUI)->Reset();
		(*pUI)->Set_Render(false);

		m_queueUI[(_uint)(*pUI)->Get_UIType()].push(*pUI);
		pUI = m_listUI[(_uint)_eUIType].erase(pUI);
	}
}

void CUIManager::Clear_UI()
{
	for (_uint i = 0; i < (_uint)UITYPE::UI_END; ++i)
	{
		for_each(m_listUI[i].begin(), m_listUI[i].end(), CDeleteObj());
		m_listUI[i].clear();
	}

	for (_uint i = 0; i < (_uint)UITYPE::UI_END; ++i)
	{
		while (!m_queueUI[i].empty())
		{
			Safe_Release(m_queueUI[i].front());
			m_queueUI[i].pop();
		}
	}
}

void CUIManager::Free()
{
	Clear_UI();
}