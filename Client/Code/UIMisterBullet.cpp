#include "pch.h"
#include "..\Header\UIMisterBullet.h"
#include "..\Header\UISniperHP.h"
#include "..\Header\UIBossCount.h"
#include "..\Header\Player.h"
#include "Export_Utility.h"

CUIMisterBullet::CUIMisterBullet(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUISniperHP(nullptr)
	, m_iCount(0)
{
	m_vecUIBossCount.clear();

	m_eUIType = UITYPE::UI_MISTERBULLET;
}

CUIMisterBullet::~CUIMisterBullet()
{
}

CUIMisterBullet* CUIMisterBullet::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIMisterBullet* pUIMisterBullet = new CUIMisterBullet(_pGraphicDev);

	if (FAILED(pUIMisterBullet->Ready_UI()))
	{
		Safe_Release(pUIMisterBullet);
		MSG_BOX("UIMisterBullet create Failed");
		return nullptr;
	}

	return pUIMisterBullet;
}

HRESULT CUIMisterBullet::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIMisterBullet::Update_UI(const _float& _fTimeDelta)
{
	if (static_cast<CPlayer*>(m_pGameObject)->Get_RightCurrState() == 6)
	{
		m_pUISniperHP->Set_Render(true);
		m_pUISniperHP->Set_HP(static_cast<CPlayer*>(m_pGameObject)->Get_PlayerHP());
	}
	else
		m_pUISniperHP->Set_Render(false);

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIMisterBullet::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIMisterBullet::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIMisterBullet::Add_Unit()
{
	m_pUISniperHP = CUISniperHP::Create(m_pGraphicDev);
	m_pUISniperHP->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUISniperHP);

	for (int i = 0; i < 20; i++)
	{
		m_vecUIBossCount.push_back(CUIBossCount::Create(m_pGraphicDev, i));
		m_vecUIBossCount[i]->Set_OwnerUI(this);

		m_vecUIUnit.push_back(m_vecUIBossCount[i]);
	}

	return S_OK;
}

void CUIMisterBullet::Add_Count()
{
	m_iCount++;

	for (int i = 0; i < m_vecUIBossCount.size(); i++)
		m_vecUIBossCount[i]->Set_Count(m_iCount);
}

void CUIMisterBullet::Reset()
{
	m_iCount = 0;

	for (int i = 0; i < m_vecUIBossCount.size(); i++)
		m_vecUIBossCount[i]->Set_Count(m_iCount);

	Engine::CUI::Reset();
}

void CUIMisterBullet::Free()
{
	Engine::CUI::Free();
}