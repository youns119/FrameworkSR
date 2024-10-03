#include "pch.h"
#include "..\Header\UIShopCard.h"
#include "Export_Utility.h"
#include "..\Header\Define.h"

CUIShopCard::CUIShopCard(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_eUIUpgrade(UI_UPGRADE::UPGRADE_END)
	, m_bStart(false)
	, m_iIndex(0)
{
	for (_uint i = 0; i < (_uint)UI_SHOPCARD::SHOPCARD_END; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUIShopCard::~CUIShopCard()
{
}

CUIShopCard* CUIShopCard::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _int _iIndex)
{
	CUIShopCard* pUIShopCard = new CUIShopCard(_pGraphicDev);

	if (FAILED(pUIShopCard->Ready_Unit(_iIndex)))
	{
		Safe_Release(pUIShopCard);
		MSG_BOX("UIShopCard Create Failed");
		return nullptr;
	}

	return pUIShopCard;
}

HRESULT CUIShopCard::Ready_Unit(_int _iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_iIndex = _iIndex;

	m_bStart = false;

	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Pos(-155.f + 260 * m_iIndex, -110.f, 0.f);

	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Scale(255.f, 255.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIShopCard::Update_Unit(const _float& _fTimeDelta)
{
	if (!m_bStart) return 0;

	On_Hover();
	On_Click();

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIShopCard::LateUpdate_Unit()
{
	if (!m_bStart) return;

	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIShopCard::Render_Unit()
{
	if (!m_bStart) return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Texture((_uint)m_eUIUpgrade);
	m_pBufferCom->Render_Buffer();

	//if (m_bStart)

}

HRESULT CUIShopCard::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIUpgrade_Card"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Base", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIUpgrade_Boarder"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Boarder", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Purchase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Purchase", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Base", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Boarder", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Purchase", pComponent });

	return S_OK;
}

void CUIShopCard::On_Click()
{
}

void CUIShopCard::On_Hover()
{
	RECT tRect{};

	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	tRect.left = -155.f + 260 * m_iIndex - 50.f;
	tRect.right = -155.f + 260 * m_iIndex + 50.f;
	tRect.bottom = -110.f - 100.f;
	tRect.top = -110.f + 100.f;

	if (PtInRect(&tRect, ptMouse))
		m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Scale(255.f * 1.1f, 255.f * 1.1f, 0.f);
}

void CUIShopCard::Reset()
{
	m_bStart = false;

	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Pos(-155.f + 260 * m_iIndex, -110.f, 0.f);

	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Scale(255.f, 255.f, 0.f);
}

void CUIShopCard::Free()
{
	Engine::CUIUnit::Free();
}