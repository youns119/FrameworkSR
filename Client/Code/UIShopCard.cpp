#include "pch.h"
#include "..\Header\UIShopCard.h"
#include "..\Header\UIShop.h"
#include "..\Header\UIInventory.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "..\Header\Define.h"

CUIShopCard::CUIShopCard(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pAnimatorCom(nullptr)
	, m_eUIUpgrade(UI_UPGRADE::UPGRADE_END)
	, m_bStart(false)
	, m_bHover(false)
	, m_bClick(false)
	, m_bSelected(false)
	, m_iIndex(0)
	, m_tRect{}
{
	for (_uint i = 0; i < (_uint)UI_SHOPCARD::SHOPCARD_END; ++i)
		m_pTextureCom[i] = nullptr;

	for (_uint i = 0; i < (_uint)UI_SHOPCARD::SHOPCARD_END - 1; ++i)
		m_pTransformCom[i] = nullptr;
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

	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Pos(-155.f + 260 * m_iIndex, -110.f, 0.f);
	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A]->Set_Pos(-155.f + 260 * m_iIndex, -110.f, 0.f);
	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE]->Set_Pos(-155.f + 260 * m_iIndex, 98.f, 0.f);

	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Scale(255.f, 255.f, 0.f);
	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A]->Set_Scale(320.f, 270.f, 0.f);
	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE]->Set_Scale(32.f, 32.f, 0.f);

	m_tRect.left = (_long)(-155.f + 260 * m_iIndex - 130.f);
	m_tRect.right = (_long)(-155.f + 260 * m_iIndex + 130.f);
	m_tRect.top = (_long)(-110.f - 230.f);
	m_tRect.bottom = (_long)(-110.f + 230.f);

	m_bRender = true;

	Set_Animation();

	return S_OK;
}

_int CUIShopCard::Update_Unit(const _float& _fTimeDelta)
{
	if (!m_bStart) return 0;

	if (static_cast<CUIShop*>(m_pOwnerUI)->Get_FinishTime() >= 1.f)
	{
		_vec3 vDir = { 0.f, -1.f, 0.f };

		for (_uint i = 0; i < (_uint)UI_SHOPCARD::SHOPCARD_END - 1; ++i)
			m_pTransformCom[i]->Move_Pos(&vDir, _fTimeDelta, static_cast<CUIShop*>(m_pOwnerUI)->Get_DownSpeed());

		return Engine::CUIUnit::Update_Unit(_fTimeDelta);
	}

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

	if (static_cast<CUIShop*>(m_pOwnerUI)->Get_Finish() && !m_bSelected)
	{
		float fAlpha(0.2f);
		DWORD tPreFactor;

		m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);
		m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Scale(255.f, 255.f, 0.f);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Change_Alpha(fAlpha);
		m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Texture((_uint)m_eUIUpgrade);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);

		return;
	}
	else if (m_bHover)
	{
		m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Scale(255.f * 1.1f, 255.f * 1.1f, 0.f);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Texture((_uint)m_eUIUpgrade);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE]->Set_Texture();
		Update_Color();
		m_pBufferCom->Render_Buffer();
	}
	else if (m_bClick)
	{
		m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Scale(255.f, 255.f, 0.f);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Texture((_uint)m_eUIUpgrade);
		m_pBufferCom->Render_Buffer();

		m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A]->Set_Scale(290.f, 243.f, 0.f);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A]->Get_WorldMatrix());

		m_pAnimatorCom->Render_Animator();
		m_pBufferCom->Render_Buffer();
	}
	else
	{
		m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Scale(255.f, 255.f, 0.f);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Texture((_uint)m_eUIUpgrade);
		m_pBufferCom->Render_Buffer();
	}
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

	pComponent = m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIUpgrade_Boarder_A"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Boarder_A", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Purchase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Purchase", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIUpgrade_Boarder"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Boarder", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Base", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Boarder_A", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Purchase", pComponent });

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Boarder", pComponent });

	return S_OK;
}

void CUIShopCard::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Boarder", m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER], 30.f);

	m_pAnimatorCom->PlayAnimation(L"Boarder", true);
}

void CUIShopCard::Update_Color()
{
	_float fHue = fmod(Engine::Get_Elapsed() * 70.f, 360.0f);
	_float fSaturation = 1.0f;
	_float fValue = 1.0f;

	m_pTextureCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE]->Change_TextureColor(fHue, fSaturation, fValue);
}

void CUIShopCard::On_Click()
{
	if (m_bClick || static_cast<CUIShop*>(m_pOwnerUI)->Get_Finish())
		return;

	if (m_bHover && Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
	{
		m_bClick = true;
		m_bHover = false;
		m_bSelected = true;

		static_cast<CUIShop*>(m_pOwnerUI)->Set_Finish(true);

		Engine::Activate_UI(UITYPE::UI_INVENTORY);
		static_cast<CUIInventory*>(Engine::Get_ListUI(UITYPE::UI_INVENTORY)->front())->Update_Inventory((_uint)m_eUIUpgrade);
		Engine::Deactivate_UI(UITYPE::UI_INVENTORY);
	}
}

void CUIShopCard::On_Hover()
{
	if (m_bClick || static_cast<CUIShop*>(m_pOwnerUI)->Get_Finish())
		return;

	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x = (_int)(ptMouse.x - WINCX / 2.f);
	ptMouse.y = (_int)(WINCY / 2.f - ptMouse.y);

	if (PtInRect(&m_tRect, ptMouse)) m_bHover = true;
	else m_bHover = false;
}

void CUIShopCard::Reset()
{
	m_bStart = false;
	m_bHover = false;
	m_bClick = false;
	m_bSelected = false;

	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Pos(-155.f + 260 * m_iIndex, -110.f, 0.f);
	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A]->Set_Pos(-155.f + 260 * m_iIndex, -110.f, 0.f);
	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE]->Set_Pos(-155.f + 260 * m_iIndex, 98.f, 0.f);

	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BASE]->Set_Scale(255.f, 255.f, 0.f);
	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_BOARDER_A]->Set_Scale(320.f, 270.f, 0.f);
	m_pTransformCom[(_uint)UI_SHOPCARD::SHOPCARD_PURCHASE]->Set_Scale(32.f, 32.f, 0.f);
}

void CUIShopCard::Free()
{
	Engine::CUIUnit::Free();
}