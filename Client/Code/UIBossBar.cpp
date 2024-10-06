#include "pch.h"
#include "..\Header\UIBossBar.h"
#include "Export_Utility.h"

CUIBossBar::CUIBossBar(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_iIndex(0)
{
}

CUIBossBar::~CUIBossBar()
{
}

CUIBossBar* CUIBossBar::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _int _iIndex)
{
	CUIBossBar* pUIBossBar = new CUIBossBar(_pGraphicDev);

	if (FAILED(pUIBossBar->Ready_Unit(_iIndex)))
	{
		Safe_Release(pUIBossBar);
		MSG_BOX("UIBossBar Create Failed");
		return nullptr;
	}

	return pUIBossBar;
}

HRESULT CUIBossBar::Ready_Unit(_int _iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_iIndex = _iIndex;

	_int iOffset = 0;

	if (m_iIndex >= 6 && m_iIndex < 12)
		iOffset = 1;
	else if (m_iIndex >= 12)
		iOffset = 2;

	m_pTransformCom->Set_Pos(-97.f + 7.f * iOffset + 12.5f * _iIndex, 160.f, 0.f);

	m_pTransformCom->Set_Scale(7.f, 23.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIBossBar::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIBossBar::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIBossBar::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIBossBar::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_Bar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Bar", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Bar", pComponent });

	return S_OK;
}

void CUIBossBar::Reset()
{
}

void CUIBossBar::Free()
{
	Engine::CUIUnit::Free();
}