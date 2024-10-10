#include "pch.h"
#include "..\Header\UIAction.h"
#include "..\Header\UIActionBase.h"
#include "Export_Utility.h"

CUIAction::CUIAction(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIActionBase(nullptr)
	, m_fDuration(0.f)
{
	m_eUIType = UITYPE::UI_ACTION;
}

CUIAction::~CUIAction()
{
}

CUIAction* CUIAction::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIAction* pUIAction = new CUIAction(_pGraphicDev);

	if (FAILED(pUIAction->Ready_UI()))
	{
		Safe_Release(pUIAction);
		MSG_BOX("UIAction create Failed");
		return nullptr;
	}

	return pUIAction;
}

HRESULT CUIAction::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIAction::Update_UI(const _float& _fTimeDelta)
{
	m_fDuration += _fTimeDelta;

	if (m_fDuration >= 5.f)
		m_bRender = false;

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIAction::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIAction::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIAction::Add_Unit()
{
	m_pUIActionBase = CUIActionBase::Create(m_pGraphicDev);
	m_pUIActionBase->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIActionBase);

	return S_OK;
}

CUIAction::ACTION_TYPE CUIAction::Get_ActionType()
{
	return (ACTION_TYPE)m_pUIActionBase->Get_ActionType();
}

void CUIAction::Set_ActionType(ACTION_TYPE _eActionType)
{
	m_pUIActionBase->Set_ActionType((_uint)_eActionType);
}

void CUIAction::Reset()
{
	m_fDuration = 0.f;

	Engine::CUI::Reset();
}

void CUIAction::Free()
{
	Engine::CUI::Free();
}