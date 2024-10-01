#include "pch.h"
#include "..\Header\UIUpgradeCard.h"
#include "Export_Utility.h"

CUIUpgradeCard::CUIUpgradeCard(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_eCurrCard(UI_CARD::CARD_BLANK)
{
	for (_uint i = 0; i < (_uint)UI_CARD::CARD_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CUIUpgradeCard::~CUIUpgradeCard()
{
}

CUIUpgradeCard* CUIUpgradeCard::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vPos)
{
	CUIUpgradeCard* pUIUpgradeCard = new CUIUpgradeCard(_pGraphicDev);

	if (FAILED(pUIUpgradeCard->Ready_Unit(_vPos)))
	{
		Safe_Release(pUIUpgradeCard);
		MSG_BOX("UIUpgradeCard create Failed");
		return nullptr;
	}

	return pUIUpgradeCard;
}

HRESULT CUIUpgradeCard::Ready_Unit(_vec3 _vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(80.f, 120.f, 0.f);
	m_pTransformCom->Set_Pos(_vPos);

	m_bRender = true;
	m_fViewZ = 9.f;

	return S_OK;
}

_int CUIUpgradeCard::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIUpgradeCard::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIUpgradeCard::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (m_eCurrCard == UI_CARD::CARD_BLANK)
		m_pTextureCom[(_uint)m_eCurrCard]->Set_Texture();

	m_pBufferCom->Render_Buffer();
}

HRESULT CUIUpgradeCard::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_CARD::CARD_BLANK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIUpgradeSlot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UIUpgradeSlot", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_CARD::CARD_WEAPON] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIUpgradeWeapon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UIUpgradeWeapon", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CUIUpgradeCard::Free()
{
	Engine::CUIUnit::Free();
}