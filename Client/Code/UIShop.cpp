#include "pch.h"
#include "..\Header\UIShop.h"
#include "..\Header\UIShopBase.h"
#include "..\Header\UIShopBuzz.h"
#include "..\Header\UIShopCard.h"
#include "Export_Utility.h"

CUIShop::CUIShop(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIShopBase(nullptr)
	, m_pUIShopBuzz(nullptr)
{
	for (int i = 0; i < 3; i++)
		m_pUIShopCard[i] = nullptr;

	m_eUIType = UITYPE::UI_SHOP;
}

CUIShop::~CUIShop()
{
}

CUIShop* CUIShop::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIShop* pUIShop = new CUIShop(_pGraphicDev);

	if (FAILED(pUIShop->Ready_UI()))
	{
		Safe_Release(pUIShop);
		MSG_BOX("UIShop create Failed");
		return nullptr;
	}

	return pUIShop;
}

HRESULT CUIShop::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIShop::Update_UI(const _float& _fTimeDelta)
{
	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIShop::LateUpdate_UI()
{
	if (static_cast<CUIShopBase*>(m_pUIShopBase)->Get_Start())
	{
		static_cast<CUIShopBuzz*>(m_pUIShopBuzz)->Set_Start(true);

		for (int i = 0; i < 3; i++)
			static_cast<CUIShopCard*>(m_pUIShopCard[i])->Set_Start(true);
	}

	Engine::CUI::LateUpdate_UI();
}

void CUIShop::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIShop::Add_Unit()
{
	CUIUnit* pUIUnit = nullptr;

	pUIUnit = m_pUIShopBase = CUIShopBase::Create(m_pGraphicDev);
	m_vecUIUnit.push_back(pUIUnit);

	pUIUnit = m_pUIShopBuzz = CUIShopBuzz::Create(m_pGraphicDev);
	m_vecUIUnit.push_back(pUIUnit);

	for (int i = 0; i < 3; i++)
	{
		pUIUnit = m_pUIShopCard[i] = CUIShopCard::Create(m_pGraphicDev, i);
		static_cast<CUIShopCard*>(m_pUIShopCard[i])->Set_Upgrade((CUIShopCard::UI_UPGRADE)i);

		m_vecUIUnit.push_back(pUIUnit);
	}

	return S_OK;
}

void CUIShop::Free()
{
	Engine::CUI::Free();
}