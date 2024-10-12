#include "pch.h"
#include "..\Header\UIRoboto.h"
#include "..\Header\UIBossLifeBar.h"
#include "Export_Utility.h"

CUIRoboto::CUIRoboto(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIBossLifeBar(nullptr)
{
	m_eUIType = UITYPE::UI_ROBOTO;
}

CUIRoboto::~CUIRoboto()
{
}

CUIRoboto* CUIRoboto::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIRoboto* pUIRoboto = new CUIRoboto(_pGraphicDev);

	if (FAILED(pUIRoboto->Ready_UI()))
	{
		Safe_Release(pUIRoboto);
		MSG_BOX("UIRoboto create Failed");
		return nullptr;
	}

	return pUIRoboto;
}

HRESULT CUIRoboto::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIRoboto::Update_UI(const _float& _fTimeDelta)
{
	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIRoboto::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIRoboto::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIRoboto::Add_Unit()
{
	m_pUIBossLifeBar = CUIBossLifeBar::Create(m_pGraphicDev);
	m_pUIBossLifeBar->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIBossLifeBar);

	return S_OK;
}

void CUIRoboto::Boss_Hit()
{
	m_pUIBossLifeBar->Boss_Hit();
}

void CUIRoboto::Free()
{
	Engine::CUI::Free();
}