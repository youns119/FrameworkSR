#include "pch.h"
#include "..\Header\UIDown.h"
#include "..\Header\UIDownBase.h"
#include "Export_Utility.h"

CUIDown::CUIDown(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIDownBase(nullptr)
	, m_fDuration(0.f)
{
	m_eUIType = UITYPE::UI_DOWN;
}

CUIDown::~CUIDown()
{
}

CUIDown* CUIDown::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIDown* pUIDown = new CUIDown(_pGraphicDev);

	if (FAILED(pUIDown->Ready_UI()))
	{
		Safe_Release(pUIDown);
		MSG_BOX("UIDown create Failed");
		return nullptr;
	}

	return pUIDown;
}

HRESULT CUIDown::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIDown::Update_UI(const _float& _fTimeDelta)
{
	m_fDuration += _fTimeDelta;

	if (m_fDuration >= 2.f)
	{
		m_bRender = false;
		return 0;
	}

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIDown::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIDown::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIDown::Add_Unit()
{
	m_pUIDownBase = CUIDownBase::Create(m_pGraphicDev);
	m_pUIDownBase->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIDownBase);

	return S_OK;
}

void CUIDown::Init(_int _iIndex)
{
	m_pUIDownBase->Init(_iIndex);

	m_fDuration -= 0.4f * _iIndex;
}

void CUIDown::Reset()
{
	m_fDuration = 0.f;

	Engine::CUI::Reset();
}

void CUIDown::Free()
{
	Engine::CUI::Free();
}