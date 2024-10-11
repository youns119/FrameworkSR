#include "pch.h"
#include "..\Header\UIPhone.h"
#include "..\Header\UIPhoneBase.h"
#include "..\Header\UIPhoneBoss.h"
#include "..\Header\Player.h"
#include "Export_Utility.h"

CUIPhone::CUIPhone(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIPhoneBase(nullptr)
	, m_pUIPhoneBoss(nullptr)
{
	m_eUIType = UITYPE::UI_PHONE;
}

CUIPhone::~CUIPhone()
{
}

CUIPhone* CUIPhone::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIPhone* pUIPhone = new CUIPhone(_pGraphicDev);

	if (FAILED(pUIPhone->Ready_UI()))
	{
		Safe_Release(pUIPhone);
		MSG_BOX("UIPhone create Failed");
		return nullptr;
	}

	return pUIPhone;
}

HRESULT CUIPhone::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIPhone::Update_UI(const _float& _fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));

	if (pPlayer->Get_BossStage())
	{
		m_pUIPhoneBase->Set_Render(false);
		m_pUIPhoneBoss->Set_Render(true);
	}
	else
	{
		m_pUIPhoneBase->Set_Render(true);
		m_pUIPhoneBoss->Set_Render(false);
	}

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIPhone::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIPhone::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIPhone::Add_Unit()
{
	m_pUIPhoneBase = CUIPhoneBase::Create(m_pGraphicDev);
	m_pUIPhoneBase->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIPhoneBase);

	m_pUIPhoneBoss = CUIPhoneBoss::Create(m_pGraphicDev);
	m_pUIPhoneBoss->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIPhoneBoss);

	return S_OK;
}

void CUIPhone::Free()
{
	Engine::CUI::Free();
}