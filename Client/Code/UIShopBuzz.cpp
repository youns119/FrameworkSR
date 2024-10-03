#include "pch.h"
#include "..\Header\UIShopBuzz.h"
#include "Export_Utility.h"

CUIShopBuzz::CUIShopBuzz(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pAnimatorCom(nullptr)
	, m_bStart(false)
{
}

CUIShopBuzz::~CUIShopBuzz()
{
}

CUIShopBuzz* CUIShopBuzz::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIShopBuzz* pUIShopBuzz = new CUIShopBuzz(_pGraphicDev);

	if (FAILED(pUIShopBuzz->Ready_Unit()))
	{
		Safe_Release(pUIShopBuzz);
		MSG_BOX("UIShopBuzz Create Failed");
		return nullptr;
	}

	return pUIShopBuzz;
}

HRESULT CUIShopBuzz::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bStart = false;

	m_pTransformCom->Set_Pos(60.f, -87.f, 0.f);

	m_pTransformCom->Set_Scale(532.f, 532.f, 0.f);

	Set_Animation();

	m_bRender = true;

	return S_OK;
}

_int CUIShopBuzz::Update_Unit(const _float& _fTimeDelta)
{
	if (!m_bStart) return 0;

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIShopBuzz::LateUpdate_Unit()
{
	if (!m_bStart) return;

	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIShopBuzz::Render_Unit()
{
	if (!m_bStart) return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (m_bStart)
		m_pAnimatorCom->Render_Animator();

	m_pBufferCom->Render_Buffer();
}

HRESULT CUIShopBuzz::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Buzz"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Buzz", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Buzz", pComponent });

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Buzz", pComponent });

	return S_OK;
}

void CUIShopBuzz::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Buzz", m_pTextureCom, 5.f);

	m_pAnimatorCom->PlayAnimation(L"Buzz", true);
}

void CUIShopBuzz::Reset()
{
	m_bStart = false;

	m_pTransformCom->Set_Pos(60.f, -87.f, 0.f);

	m_pTransformCom->Set_Scale(532.f, 532.f, 0.f);

	m_pAnimatorCom->GetCurrAnim()->ResetAnimFrame(0.f);
}

void CUIShopBuzz::Free()
{
	Engine::CUIUnit::Free();
}