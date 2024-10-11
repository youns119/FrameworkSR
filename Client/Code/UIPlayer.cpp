#include "pch.h"
#include "..\Header\UIPlayer.h"
#include "..\Header\UIReload.h"
#include "..\Header\UIItem.h"
#include "..\Header\UIEmergency.h"
#include "..\Header\Player.h"
#include "Export_Utility.h"

CUIPlayer::CUIPlayer(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIReload(nullptr)
	, m_pUIItem(nullptr)
	, m_pUIEmergency(nullptr)
{
	m_eUIType = UITYPE::UI_PLAYER;
}

CUIPlayer::~CUIPlayer()
{
}

CUIPlayer* CUIPlayer::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIPlayer* pUIPlayer = new CUIPlayer(_pGraphicDev);

	if (FAILED(pUIPlayer->Ready_UI()))
	{
		Safe_Release(pUIPlayer);
		MSG_BOX("UIPlayer create Failed");
		return nullptr;
	}

	return pUIPlayer;
}

HRESULT CUIPlayer::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIPlayer::Update_UI(const _float& _fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));

	if (pPlayer->Get_BossStage())
		m_pUIEmergency->Set_Render(false);
	else m_pUIEmergency->Set_Render(true);

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIPlayer::LateUpdate_UI()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));

	if (pPlayer->Get_HasItem())
		m_pUIItem->Set_Render(true);
	else m_pUIItem->Reset();

	Engine::CUI::LateUpdate_UI();
}

void CUIPlayer::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIPlayer::Add_Unit()
{
	m_pUIReload = CUIReload::Create(m_pGraphicDev);
	m_pUIReload->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIReload);

	m_pUIItem = CUIItem::Create(m_pGraphicDev);
	m_pUIItem->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIItem);

	m_pUIEmergency = CUIEmergency::Create(m_pGraphicDev);
	m_pUIEmergency->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUIEmergency);

	return S_OK;
}

void CUIPlayer::Free()
{
	Engine::CUI::Free();
}