#include "pch.h"
#include "..\Header\UIUpgradeCard.h"
#include "Export_Utility.h"
#include "..\Header\Define.h"

CUIUpgradeCard::CUIUpgradeCard(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_eCurrCard(UI_CARD::CARD_BLANK)
	, m_bHover(false)
	, m_tRect{}
{
	for (_uint i = 0; i < (_uint)UI_UPGRADE::UPGRADE_END; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
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

	m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_CARD]->Set_Pos(_vPos);
	m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_BOARDER]->Set_Pos(_vPos);

	m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_CARD]->Set_Scale(150.f, 150.f, 0.f);
	m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_BOARDER]->Set_Scale(210.f, 190.f, 0.f);

	m_tRect.left = (_long)(_vPos.x - 80.f);
	m_tRect.right = (_long)(_vPos.x + 80.f);
	m_tRect.top = (_long)(_vPos.y - 125.f);
	m_tRect.bottom = (_long)(_vPos.y + 125.f);

	m_bRender = true;

	return S_OK;
}

_int CUIUpgradeCard::Update_Unit(const _float& _fTimeDelta)
{
	On_Hover();

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIUpgradeCard::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIUpgradeCard::Render_Unit()
{
	if (m_eCurrCard != UI_CARD::CARD_BLANK && m_bHover)
	{
		m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_CARD]->Set_Scale(150.f * 1.2f, 150.f * 1.2f, 0.f);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_CARD]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_UPGRADE::UPGRADE_CARD]->Set_Texture((int)m_eCurrCard);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_BOARDER]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_UPGRADE::UPGRADE_BOARDER]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
	else
	{
		m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_CARD]->Set_Scale(150.f, 150.f, 0.f);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_CARD]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_UPGRADE::UPGRADE_CARD]->Set_Texture((int)m_eCurrCard);
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUIUpgradeCard::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_UPGRADE::UPGRADE_CARD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIUpgrade_Card"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UIUpgrade_Card", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_UPGRADE::UPGRADE_BOARDER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIUpgrade_Boarder_A"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UIUpgrade_Boarder", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_CARD] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Card", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_BOARDER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Boarder", pComponent });

	return S_OK;
}

void CUIUpgradeCard::On_Hover()
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x = (_int)(ptMouse.x - WINCX / 2.f);
	ptMouse.y = (_int)(WINCY / 2.f - ptMouse.y);

	if (PtInRect(&m_tRect, ptMouse)) m_bHover = true;
	else m_bHover = false;
}

void CUIUpgradeCard::Reset()
{
	m_bHover = false;

	m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_CARD]->Set_Scale(150.f, 150.f, 0.f);
	m_pTransformCom[(_uint)UI_UPGRADE::UPGRADE_BOARDER]->Set_Scale(210.f, 190.f, 0.f);
}

void CUIUpgradeCard::Free()
{
	Engine::CUIUnit::Free();
}