#include "pch.h"
#include "..\Header\UIOverlay.h"
#include "Export_Utility.h"

CUIOverlay::CUIOverlay(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
{
	for (_uint i = 0; i < (_uint)UI_OVERLAY::OVERLAY_END; ++i)
		m_pTextureCom[i] = nullptr;

	m_eUIType = UITYPE::UI_UPGRADE;
}

CUIOverlay::~CUIOverlay()
{
}

CUIOverlay* CUIOverlay::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIOverlay* pUIOverlay = new CUIOverlay(_pGraphicDev);

	if (FAILED(pUIOverlay->Ready_UI()))
	{
		Safe_Release(pUIOverlay);
		MSG_BOX("UIOverlay create Failed");
		return nullptr;
	}

	return pUIOverlay;
}

HRESULT CUIOverlay::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(WINCX / 2.f, WINCY / 2.f, 0.f);
	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIOverlay::Update_UI(const _float& _fTimeDelta)
{
	if (!m_bRender)
		return 0;

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIOverlay::LateUpdate_UI()
{
	if (!m_bRender)
		return;

	Engine::CUI::LateUpdate_UI();
}

void CUIOverlay::Render_UI()
{
	float fAlpha1, fAlpha2 = 0.1f;
	fAlpha1 = 0.3f; fAlpha2 = 0.6f;
	DWORD preFactor, textureFactor;
	textureFactor = D3DCOLOR_ARGB((int)(fAlpha1 * 255), 255, 255, 255);
	m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &preFactor);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, textureFactor);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BACK]->Set_Texture();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pBufferCom->Render_Buffer();

	textureFactor = D3DCOLOR_ARGB((int)(fAlpha2 * 255), 255, 255, 255);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, textureFactor);

	m_pTextureCom[(_uint)UI_OVERLAY::OVERLAY_BASE]->Set_Texture();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, preFactor);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
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
	Engine::CUI::Free();
}