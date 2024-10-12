#include "pch.h"
#include "..\Header\UIShop.h"
#include "..\Header\UIShopBase.h"
#include "..\Header\UIShopBuzz.h"
#include "..\Header\UIShopCard.h"
#include "..\Header\Player.h"
#include "Export_Utility.h"

CUIShop::CUIShop(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIShopBase(nullptr)
	, m_pUIShopBuzz(nullptr)
	, m_bFinished(false)
	, m_fFinishTime(0.f)
	, m_fDownSpeed(0.f)
{
	m_vecUIShopCard.clear();

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

	m_fDownSpeed = 1000.f;

	return S_OK;
}

_int CUIShop::Update_UI(const _float& _fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));

	if (pPlayer->Get_CurrFloor() == 3)
	{
		m_vecUIShopCard[0]->Set_Upgrade(CUIShopCard::UI_UPGRADE::UPGRADE_SLOWBULLET);
		m_vecUIShopCard[1]->Set_Upgrade(CUIShopCard::UI_UPGRADE::UPGRADE_MASCULINITY);
		m_vecUIShopCard[2]->Set_Upgrade(CUIShopCard::UI_UPGRADE::UPGRADE_BOSSKILLER);
	}
	else if (pPlayer->Get_BossStage())
	{
		m_vecUIShopCard[0]->Set_Upgrade(CUIShopCard::UI_UPGRADE::UPGRADE_MASCULINITY);
		m_vecUIShopCard[1]->Set_Upgrade(CUIShopCard::UI_UPGRADE::UPGRADE_SNIPER);
		m_vecUIShopCard[2]->Set_Upgrade(CUIShopCard::UI_UPGRADE::UPGRADE_SLOWBULLET);
	}

	if (m_fFinishTime >= 2.f)
	{
		m_bRender = false;
		return 0;
	}

	if (m_bFinished)
		m_fFinishTime += _fTimeDelta;

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIShop::LateUpdate_UI()
{
	if (m_pUIShopBase->Get_Start())
	{
		for (int i = 0; i < m_vecUIShopCard.size(); i++)
			m_vecUIShopCard[i]->Set_Start(true);

		m_pUIShopBuzz->Set_Render(true);
	}

	Engine::CUI::LateUpdate_UI();
}

void CUIShop::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIShop::Add_Unit()
{
	m_pUIShopBase = CUIShopBase::Create(m_pGraphicDev);
	m_pUIShopBase->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIShopBase);

	for (int i = 0; i < 3; i++)
	{
		m_vecUIShopCard.push_back(CUIShopCard::Create(m_pGraphicDev, i));
		m_vecUIShopCard[i]->Set_Upgrade((CUIShopCard::UI_UPGRADE)i);
		m_vecUIShopCard[i]->Set_OwnerUI(this);

		m_vecUIUnit.push_back(m_vecUIShopCard[i]);
	}

	m_pUIShopBuzz = CUIShopBuzz::Create(m_pGraphicDev);
	m_pUIShopBuzz->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIShopBuzz);

	return S_OK;
}

void CUIShop::Reset()
{
	m_bFinished = false;
	m_fFinishTime = 0.f;

	CUI::Reset();
}

void CUIShop::Free()
{
	Engine::CUI::Free();
}