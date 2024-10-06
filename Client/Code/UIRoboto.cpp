#include "pch.h"
#include "..\Header\UIRoboto.h"
#include "..\Header\UIBossLifeBar.h"
#include "..\Header\UIBossBar.h"
#include "..\Header\UIBossName.h"
#include "..\Header\Boss_Robot.h"
#include "Export_Utility.h"

CUIRoboto::CUIRoboto(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIBossLifeBar(nullptr)
	, m_pUIBossName(nullptr)
	, m_fMaxHP(0.f)
	, m_fCurrHP(0.f)
{
	m_vecUIBossBar.clear();

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
	//CBoss_Robot* pRoboto = static_cast<CBoss_Robot*>(Engine::Get_CurrScene()->Get_GameObject(L"Monster", L"Boss_Robot"));

	//m_fMaxHP = pRoboto->Get_BossHP();
	//m_fCurrHP = pRoboto->Get

	//for (int i = 0; i < m_vecUIBossBar.size(); ++i)
	//{
	//	if(m_fHP >= m_)

	//	m_vecUIBossBar[i]->Set_Render(false);
	//}

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

	for (int i = 0; i < 18; i++)
	{
		m_vecUIBossBar.push_back(CUIBossBar::Create(m_pGraphicDev, i));
		m_vecUIBossBar[i]->Set_OwnerUI(this);
		m_vecUIUnit.push_back(m_vecUIBossBar[i]);
	}

	m_pUIBossName = CUIBossName::Create(m_pGraphicDev);
	m_pUIBossName->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIBossName);

	return S_OK;
}

void CUIRoboto::Reset()
{
	Engine::CUI::Reset();
}

void CUIRoboto::Free()
{
	Engine::CUI::Free();
}