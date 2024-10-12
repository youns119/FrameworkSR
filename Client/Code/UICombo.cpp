#include "pch.h"
#include "..\Header\UICombo.h"
#include "..\Header\UIComboBase.h"

CUICombo::CUICombo(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIComboBase(nullptr)
{
	m_eUIType = UITYPE::UI_COMBO;
}

CUICombo::~CUICombo()
{
}

CUICombo* CUICombo::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUICombo* pUICombo = new CUICombo(_pGraphicDev);

	if (FAILED(pUICombo->Ready_UI()))
	{
		Safe_Release(pUICombo);
		MSG_BOX("UICombo create Failed");
		return nullptr;
	}

	return pUICombo;
}

HRESULT CUICombo::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUICombo::Update_UI(const _float& _fTimeDelta)
{
	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUICombo::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUICombo::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUICombo::Add_Unit()
{
	m_pUIComboBase = CUIComboBase::Create(m_pGraphicDev);
	m_pUIComboBase->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIComboBase);

	return S_OK;
}

_int CUICombo::Get_Combo()
{
	return m_pUIComboBase->Get_Combo();
}

void CUICombo::Combo_Up()
{
	m_pUIComboBase->Combo_Up();
}

void CUICombo::Free()
{
	Engine::CUI::Free();
}