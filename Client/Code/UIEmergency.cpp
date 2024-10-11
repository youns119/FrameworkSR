#include "pch.h"
#include "..\Header\UIEmergency.h"
#include "..\Header\Player.h"
#include "Export_Utility.h"

CUIEmergency::CUIEmergency(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_bActive(false)
{
}

CUIEmergency::~CUIEmergency()
{
}

CUIEmergency* CUIEmergency::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIEmergency* pUIEmergency = new CUIEmergency(_pGraphicDev);

	if (FAILED(pUIEmergency->Ready_Unit()))
	{
		Safe_Release(pUIEmergency);
		MSG_BOX("UIEmergency create Failed");
		return nullptr;
	}

	return pUIEmergency;
}

HRESULT CUIEmergency::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(0.f, WINCY / 2.f - 200.f, 0.f);

	m_pTransformCom->Set_Scale(130.f, 130.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIEmergency::Update_Unit(const _float& _fTimeDelta)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));
	m_fTimerHP = pPlayer->Get_TimerHP();

	if (m_fTimerHP >= 4.f || m_fTimerHP <= 0.f) m_bActive = false;
	else m_bActive = true;

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIEmergency::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIEmergency::Render_Unit()
{
	if (!m_bActive) return;

	_float fAlpha = 0.8f;
	DWORD tPreFactor;
	m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture((_uint)(m_fTimerHP - 1.f));
	m_pTextureCom->Change_Alpha(fAlpha);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
}

HRESULT CUIEmergency::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIEmergency_Number"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Number", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Number", pComponent });

	return S_OK;
}

void CUIEmergency::Free()
{
	Engine::CUIUnit::Free();
}