#include "pch.h"
#include "..\Header\UIShopBase.h"
#include "Export_Utility.h"

CUIShopBase::CUIShopBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_fScaleRatio(0.f)
{
	for (_uint i = 0; i < (_uint)UI_SHOPBASE::SHOPBASE_END; ++i)
		m_pTextureCom[i] = nullptr;

	for (int i = 0; i < 2; ++i)
		m_pTransformCom[i] = nullptr;
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

	m_pTransformCom[0]->Set_Pos(0.f, 0.f, 0.f);

	m_pTransformCom[0]->Set_Scale(850.f, 850.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIShopBase::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIShopBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIShopBase::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SHOPBASE::SHOPBASE_HAND]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_HAND]->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIShopBase::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_HAND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Hand"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Hand", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_BADGE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Badge"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Badge", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_STATIC] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Static"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Static", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_PHONE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_Base"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_PhoneBase", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_BUZZ] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Buzz"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Buzz", pComponent });

	pComponent = m_pTransformCom[0] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Hand", pComponent });

	pComponent = m_pTransformCom[1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Screen", pComponent });

	return S_OK;
}

void CUIShopBase::Free()
{
	Engine::CUIUnit::Free();
}