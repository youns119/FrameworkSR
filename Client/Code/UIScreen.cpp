#include "pch.h"
#include "..\Header\UIScreen.h"
#include "..\Header\UIScreenBase.h"
#include "..\Header\UIShop.h"
#include "..\Header\UIVictory.h"
#include "Export_Utility.h"
#include "Export_System.h"

CUIScreen::CUIScreen(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIScreenBase(nullptr)
	, m_bShop(false)
	, m_fShopTime(0.f)
{
	m_eUIType = UITYPE::UI_SCREEN;
}

CUIScreen::~CUIScreen()
{
}

CUIScreen* CUIScreen::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIScreen* pUIScreen = new CUIScreen(_pGraphicDev);

	if (FAILED(pUIScreen->Ready_UI()))
	{
		Safe_Release(pUIScreen);
		MSG_BOX("UIScreen create Failed");
		return nullptr;
	}

	return pUIScreen;
}

HRESULT CUIScreen::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIScreen::Update_UI(const _float& _fTimeDelta)
{
	m_fShopTime += _fTimeDelta;

	Engine::Set_ChangeScreen(true);

	if (m_fShopTime >= 3.f)
		if (!m_bShop)
		{
			Engine::Activate_UI(UITYPE::UI_SHOP);
			m_bShop = true;

			Play_Sound(L"Shop_Enter.wav", CHANNELID::SOUND_INTERFACE, 0.7f);
		}

	if (m_pUIScreenBase->Get_BackToNormal())
	{
		m_bRender = false;

		if (!Engine::Get_ListUI(UITYPE::UI_VICTORY)->empty())
			static_cast<CUIVictory*>(Engine::Get_ListUI(UITYPE::UI_VICTORY)->front())->Set_Render(false);

		Engine::Set_ChangeScreen(false);
	}

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIScreen::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIScreen::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIScreen::Add_Unit()
{
	m_pUIScreenBase = CUIScreenBase::Create(m_pGraphicDev);
	m_pUIScreenBase->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIScreenBase);

	return S_OK;
}

_int CUIScreen::Get_Floor()
{
	return m_pUIScreenBase->Get_Floor();
}

void CUIScreen::Set_Return(_bool _bReturn)
{
	m_pUIScreenBase->Set_Return(_bReturn);
}

void CUIScreen::Set_Floor(_int _iFloor)
{
	m_pUIScreenBase->Set_Floor(_iFloor);
}

void CUIScreen::Reset()
{
	Engine::Set_ChangeScreen(false);

	m_bShop = false;
	m_fShopTime = 0.f;

	Engine::CUI::Reset();
}

void CUIScreen::Free()
{
	Engine::CUI::Free();
}