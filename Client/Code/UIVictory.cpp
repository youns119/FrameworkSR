#include "pch.h"
#include "..\Header\UIVictory.h"
#include "..\Header\UIVictoryBase.h"

CUIVictory::CUIVictory(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIVictoryBase(nullptr)
{
	m_eUIType = UITYPE::UI_VICTORY;
}

CUIVictory::~CUIVictory()
{
}

CUIVictory* CUIVictory::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIVictory* pUIVictory = new CUIVictory(_pGraphicDev);

	if (FAILED(pUIVictory->Ready_UI()))
	{
		Safe_Release(pUIVictory);
		MSG_BOX("UIVictory create Failed");
		return nullptr;
	}

	return pUIVictory;
}

HRESULT CUIVictory::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIVictory::Update_UI(const _float& _fTimeDelta)
{
	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIVictory::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIVictory::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIVictory::Add_Unit()
{
	m_pUIVictoryBase = CUIVictoryBase::Create(m_pGraphicDev);
	m_pUIVictoryBase->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIVictoryBase);

	return S_OK;
}

void CUIVictory::Set_FloorTime(_float _fFloorTime)
{
	m_pUIVictoryBase->Set_FloorTime(_fFloorTime);
}

void CUIVictory::Free()
{
	Engine::CUI::Free();
}