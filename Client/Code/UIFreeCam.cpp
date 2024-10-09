#include "pch.h"
#include "..\Header\UIFreeCam.h"
#include "..\Header\UIIndicator.h"
#include "..\Header\Player.h"
#include "Export_Utility.h"

CUIFreeCam::CUIFreeCam(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pIndicator(nullptr)
{
	m_eUIType = UITYPE::UI_FREECAM;
}

CUIFreeCam::~CUIFreeCam()
{
}

CUIFreeCam* CUIFreeCam::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIFreeCam* pUIFreeCam = new CUIFreeCam(_pGraphicDev);

	if (FAILED(pUIFreeCam->Ready_UI()))
	{
		Safe_Release(pUIFreeCam);
		MSG_BOX("UIInventory create Failed");
		return nullptr;
	}

	return pUIFreeCam;
}

HRESULT CUIFreeCam::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUIFreeCam::Update_UI(const _float& _fTimeDelta)
{
	_vec3 vPos{};
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));
	CComponent* pComponent = pPlayer->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Body_Transform");
	static_cast<CTransform*>(pComponent)->Get_Info(INFO::INFO_POS, &vPos);

	m_pIndicator->Set_Pos(vPos);

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIFreeCam::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIFreeCam::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUIFreeCam::Add_Unit()
{
	m_pIndicator = CUIIndicator::Create(m_pGraphicDev);
	m_pIndicator->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pIndicator);

	return S_OK;
}

void CUIFreeCam::Free()
{
	Engine::CUI::Free();
}