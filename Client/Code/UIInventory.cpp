#include "pch.h"
#include "..\Header\UIInventory.h"
#include "..\Header\UIOverlay.h"
#include "..\Header\UIUpgradeCard.h"
#include "Export_Utility.h"

CUIInventory::CUIInventory(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pOverlay(nullptr)
{
	m_vecUpgradeCard.clear();

	m_eUIType = UITYPE::UI_INVENTORY;
}

CUIInventory::~CUIInventory()
{
}

CUIInventory* CUIInventory::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIInventory* pUIInventory = new CUIInventory(_pGraphicDev);

	if (FAILED(pUIInventory->Ready_UI()))
	{
		Safe_Release(pUIInventory);
		MSG_BOX("UIInventory create Failed");
		return nullptr;
	}

	return pUIInventory;
}

HRESULT CUIInventory::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIInventory::Update_UI(const _float& _fTimeDelta)
{
	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIInventory::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIInventory::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIInventory::Add_Unit()
{
	CUIUnit* pUIUnit = nullptr;

	pUIUnit = m_pOverlay = CUIOverlay::Create(m_pGraphicDev);
	m_vecUIUnit.push_back(pUIUnit);

	for (int i = 0; i < 10; i++)
	{
		_vec3 vPos;

		if (i < 5) vPos = { -WINCX / 2.f + (WINCX / 6.f) * (i + 1), 100.f, 0.f };
		else vPos = { -WINCX / 2.f + (WINCX / 6.f) * (i - 4), -200.f, 0.f };

		pUIUnit = CUIUpgradeCard::Create(m_pGraphicDev, vPos);
		m_vecUIUnit.push_back(pUIUnit);
		m_vecUpgradeCard.push_back(pUIUnit);
	}

	return S_OK;
}

void CUIInventory::Free()
{
	Engine::CUI::Free();
}