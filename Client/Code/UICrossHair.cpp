#include "pch.h"
#include "..\Header\UICrossHair.h"
#include "Export_Utility.h"
#include "..\Header\Player.h"

CUICrossHair::CUICrossHair(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_eCurrCrossHair(UI_CROSSHAIR::CROSSHAIR_END)
{
	for (_uint i = 0; i < (_uint)UI_CROSSHAIR::CROSSHAIR_END; ++i)
		m_pTextureCom[i] = nullptr;

	m_eUIType = UITYPE::UI_CROSSHAIR;
}

CUICrossHair::~CUICrossHair()
{
}

CUICrossHair* CUICrossHair::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUICrossHair* pUICrossHair = new CUICrossHair(_pGraphicDev);

	if (FAILED(pUICrossHair->Ready_UI()))
	{
		Safe_Release(pUICrossHair);
		MSG_BOX("UICrossHair create Failed");
		return nullptr;
	}

	return pUICrossHair;
}

HRESULT CUICrossHair::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(20.f, 20.f, 0.f);
	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUICrossHair::Update_UI(const _float& _fTimeDelta)
{
	if (!m_bRender)
		return 0;

	if (Engine::Get_UILayerRender(UITYPE::UI_UPGRADE))
	{
		POINT tMouse{};

		GetCursorPos(&tMouse);
		ScreenToClient(g_hWnd, &tMouse);

		m_pTransformCom->Set_Pos((_float)(tMouse.x - WINCX / 2.f), (_float)(WINCY / 2.f - tMouse.y), 0.f);
	}

	if (Engine::Get_UILayerRender(UITYPE::UI_UPGRADE))
		m_eCurrCrossHair = (UI_CROSSHAIR::CROSSHAIR_PISTOL);
	else
	{
		int iHairNum = static_cast<CPlayer*>(m_pGameObject)->Get_WeaponState();
		m_eCurrCrossHair = (UI_CROSSHAIR)iHairNum;
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	}

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUICrossHair::LateUpdate_UI()
{
	if (!m_bRender)
		return;

	Engine::CUI::LateUpdate_UI();
}

void CUICrossHair::Render_UI()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom[(_uint)m_eCurrCrossHair]->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUICrossHair::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_CROSSHAIR::CROSSHAIR_PISTOL] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICrossHair_Pistol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UICrossHair_Pistol", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_CROSSHAIR::CROSSHAIR_RIFLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICrossHair_Rifle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UICrossHair_Rifle", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_CROSSHAIR::CROSSHAIR_SNIPER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICrossHair_Sniper"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UICrossHair_Sniper", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_CROSSHAIR::CROSSHAIR_SHOTGUN] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICrossHair_ShotGun"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UICrossHair_ShotGun", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_CROSSHAIR::CROSSHAIR_KATANA] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICrossHair_Katana"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UICrossHair_Katana", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_CROSSHAIR::CROSSHAIR_MINIGUN] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICrossHair_MiniGun"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UICrossHair_MiniGun", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CUICrossHair::Free()
{
	Engine::CUI::Free();
}