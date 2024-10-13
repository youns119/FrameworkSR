#include "pch.h"
#include "..\Header\UIEnding.h"
#include "..\Header\UIEndingBase.h"

CUIEnding::CUIEnding(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIEndingBase(nullptr)
{
	m_eUIType = UITYPE::UI_ENDING;
}

CUIEnding::~CUIEnding()
{
}

CUIEnding* CUIEnding::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIEnding* pUIEnding = new CUIEnding(_pGraphicDev);

	if (FAILED(pUIEnding->Ready_UI()))
	{
		Safe_Release(pUIEnding);
		MSG_BOX("UIEnding create Failed");
		return nullptr;
	}

	return pUIEnding;
}

HRESULT CUIEnding::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIEnding::Update_UI(const _float& _fTimeDelta)
{
	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIEnding::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIEnding::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIEnding::Add_Unit()
{
	m_pUIEndingBase = CUIEndingBase::Create(m_pGraphicDev);
	m_pUIEndingBase->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIEndingBase);

	return S_OK;
}

void CUIEnding::Free()
{
	Engine::CUI::Free();
}