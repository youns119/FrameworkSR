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

	for (_uint i = 0; i < 10; i++)
		m_vecInventory.push_back((_uint)CUIUpgradeCard::UI_CARD::CARD_BLANK);

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
	m_pOverlay = CUIOverlay::Create(m_pGraphicDev);
	m_pOverlay->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pOverlay);

	for (int i = 0; i < 10; i++)
	{
		_vec3 vPos;

		if (i < 5) vPos = { -WINCX / 2.f + (WINCX / 6.f) * (i + 1), 70.f, 0.f };
		else vPos = { -WINCX / 2.f + (WINCX / 6.f) * (i - 4), -200.f, 0.f };

		m_vecUpgradeCard.push_back(CUIUpgradeCard::Create(m_pGraphicDev, vPos));
		m_vecUpgradeCard[i]->Set_OwnerUI(this);
		m_vecUIUnit.push_back(m_vecUpgradeCard[i]);
	}

	return S_OK;
}

void CUIInventory::Update_Inventory(_uint _iIndex)
{
	for (int i = 0; i < m_vecUpgradeCard.size(); i++)
		if (m_vecUpgradeCard[i]->Get_CurrCard() == CUIUpgradeCard::UI_CARD::CARD_BLANK)
		{
			m_vecUpgradeCard[i]->Set_CurrCard((CUIUpgradeCard::UI_CARD)_iIndex);
			break;
		}
}

void CUIInventory::Free()
{
	Engine::CUI::Free();
}