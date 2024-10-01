#include "pch.h"
#include "..\Header\UIOverlay.h"
#include "Export_Utility.h"

CUIOverlay::CUIOverlay(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
{
	for (_uint i = 0; i < (_uint)UI_OVERLAY::OVERLAY_END; ++i)
		m_pTextureCom[i] = nullptr;
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

	m_pTransformCom->Set_Scale(WINCX / 2.f, WINCY / 2.f, 0.f);
	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);

	m_bRender = true;
	m_fViewZ = 10.f;

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
	float fAlpha1(0.3f), fAlpha2(0.6f);
	DWORD tPreFactor;

	m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BACK]->Change_Alpha(fAlpha1);
	m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BACK]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BACK]->Change_Alpha(fAlpha2);
	m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BASE]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
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

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CUIOverlay::Free()
{
	Engine::CUIUnit::Free();
}