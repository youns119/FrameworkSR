#include "pch.h"
#include "..\Header\UIOverlay.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIOverlay::CUIOverlay(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
{
	for (_uint i = 0; i < (_uint)UI_OVERLAY::OVERLAY_END; ++i)
		m_pTextureCom[i] = nullptr;

	for (int i = 0; i < 5; i++)
		m_pTransformCom[i] = nullptr;
}

CUIOverlay::~CUIOverlay()
{
}

CUIOverlay* CUIOverlay::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIOverlay* pUIOverlay = new CUIOverlay(_pGraphicDev);

	if (FAILED(pUIOverlay->Ready_Unit()))
	{
		Safe_Release(pUIOverlay);
		MSG_BOX("UIOverlay create Failed");
		return nullptr;
	}

	return pUIOverlay;
}

HRESULT CUIOverlay::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom[0]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[1]->Set_Pos(-WINCX / 2.f + 50.f, WINCY / 2.f - 50.f, 0.f);
	m_pTransformCom[2]->Set_Pos(-WINCX / 2.f + 170.f, WINCY / 2.f - 50.f, 0.f);
	m_pTransformCom[3]->Set_Pos(5.f, WINCY / 2.f - 75.f, 0.f);
	m_pTransformCom[4]->Set_Pos(0.f, WINCY / 2.f - 70.f, 0.f);

	m_pTransformCom[0]->Set_Scale(WINCX / 2.f, WINCY / 2.f, 0.f);
	m_pTransformCom[1]->Set_Scale(10.f, 10.f, 0.f);
	m_pTransformCom[2]->Set_Scale(100.f, 100.f, 0.f);
	m_pTransformCom[3]->Set_Scale(250.f, 250.f, 0.f);
	m_pTransformCom[4]->Set_Scale(250.f, 250.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIOverlay::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIOverlay::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIOverlay::Render_Unit()
{
	{
		float fAlpha1(0.3f), fAlpha2(0.6f), fAlpha3(0.7f), fAlpha4(1.f);
		DWORD tPreFactor;

		m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[0]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BACK]->Change_Alpha(fAlpha1);
		m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BACK]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BACK]->Change_Alpha(fAlpha2);
		m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BASE]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[3]->Get_WorldMatrix());
		m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_MODERATOR]->Change_Alpha(fAlpha3);
		m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_MODERATOR]->Set_Texture();
		Update_Color(m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_MODERATOR]);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
	}

	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[1]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_ICON]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[2]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_LIVESTREAM]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[4]->Get_WorldMatrix());
		m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_MODERATOR_A]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUIOverlay::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UIBack", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BASE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIOverlay"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UIOverlay", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_ICON] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UILiveIcon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UILiveIcon", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_LIVESTREAM] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_LiveStream"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UILiveStream", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_MODERATOR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Moderator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UIModerator", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_MODERATOR_A] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Moderator_A"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_UIModerator_A", pComponent });

	pComponent = m_pTransformCom[0] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Overlay", pComponent });

	pComponent = m_pTransformCom[1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Icon", pComponent });

	pComponent = m_pTransformCom[2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_LiveStream", pComponent });

	pComponent = m_pTransformCom[3] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Moderator", pComponent });

	pComponent = m_pTransformCom[4] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Moderator_A", pComponent });

	return S_OK;
}

void CUIOverlay::Update_Color(CTexture* _pTexture)
{
	_float fHue = fmod(Engine::Get_Elapsed_Fixed() * 70.f, 360.0f);
	_float fSaturation = 1.0f;
	_float fValue = 1.0f;

	_pTexture->Change_TextureColor(fHue, fSaturation, fValue);
}

void CUIOverlay::Free()
{
	Engine::CUIUnit::Free();
}