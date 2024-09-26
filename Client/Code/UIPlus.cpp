#include "pch.h"
#include "..\Header\UIPlus.h"
#include "Export_Utility.h"

CUIPlus::CUIPlus(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_eCurrPlus(UI_PLUS::PLUS_END)
{
	m_eUIType = UITYPE::UI_PLUS;
}

CUIPlus::~CUIPlus()
{
}

CUIPlus* CUIPlus::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIPlus* pUIPlus = new CUIPlus(_pGraphicDev);

	if (FAILED(pUIPlus->Ready_UI()))
	{
		Safe_Release(pUIPlus);
		MSG_BOX("UIPlus Create Failed");
		return nullptr;
	}

	return pUIPlus;
}

HRESULT CUIPlus::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(0.f, 200.f, 0.f);
	m_pTransformCom->Set_Scale(50.f, 20.f, 0.f);

	m_bRender = false;

	return S_OK;
}

_int CUIPlus::Update_UI(const _float& _fTimeDelta)
{
	if (!m_bRender)
		return 0;

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIPlus::LateUpdate_UI()
{
	if (!m_bRender)
		return;

	Engine::CUI::LateUpdate_UI();
}

void CUIPlus::Render_UI()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIPlus::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPlus"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CUIPlus::Free()
{
	Engine::CUI::Free();
}