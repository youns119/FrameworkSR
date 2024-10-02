#include "pch.h"
#include "..\Header\UIShopBase.h"
#include "Export_Utility.h"

CUIShopBase::CUIShopBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pAnimatorCom(nullptr)
	, m_fScaleRatio(0.f)
{
	for (_uint i = 0; i < (_uint)UI_SHOPBASE::SHOPBASE_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CUIShopBase::~CUIShopBase()
{
}

CUIShopBase* CUIShopBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIShopBase* pUIShopBase = new CUIShopBase(_pGraphicDev);

	if (FAILED(pUIShopBase->Ready_Unit()))
	{
		Safe_Release(pUIShopBase);
		MSG_BOX("UIShopBase Create Failed");
		return nullptr;
	}

	return pUIShopBase;
}

HRESULT CUIShopBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(0.f, -(WINCY / 2.f + 850.f), 0.f);

	m_pTransformCom->Set_Scale(850.f, 850.f, 0.f);

	Set_Animation();

	m_bRender = true;

	return S_OK;
}

_int CUIShopBase::Update_Unit(const _float& _fTimeDelta)
{
	_vec3 vPos;
	_vec3 vDir = { 0.f, 1.f, 0.f };

	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	if (vPos.y < 0.f)
	{

	}
	m_pTransformCom->Move_Pos(&vDir, _fTimeDelta, 3.f);




	if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
		m_pAnimatorCom->PlayAnimation(L"Load", true);

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIShopBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIShopBase::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pAnimatorCom->Render_Animator();
	m_pBufferCom->Render_Buffer();

	//m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_HAND]->Set_Texture();
}

HRESULT CUIShopBase::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_START] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Hand_Start"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Start", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_LOAD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Hand_Load"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Load", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_BASE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Hand_Base"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Base", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Shop", pComponent });

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Shop", pComponent });

	return S_OK;
}

void CUIShopBase::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Start", m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_START], 7.f);
	m_pAnimatorCom->CreateAnimation(L"Load", m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_LOAD], 7.f);

	m_pAnimatorCom->PlayAnimation(L"Start", false);
}

void CUIShopBase::Reset()
{
	m_pAnimatorCom->GetCurrAnim()->ResetAnimFrame(0.f);

	m_pAnimatorCom->PlayAnimation(L"Start", false);
	m_pAnimatorCom->GetCurrAnim()->ResetAnimFrame(0.f);
}

void CUIShopBase::Free()
{
	Engine::CUIUnit::Free();
}