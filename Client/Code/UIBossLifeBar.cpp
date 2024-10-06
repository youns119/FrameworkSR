#include "pch.h"
#include "..\Header\UIBossLifeBar.h"
#include "Export_Utility.h"

CUIBossLifeBar::CUIBossLifeBar(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
{
	for (_uint i = 0; i < (_uint)UI_LIFEBAR::LIFEBAR_END; ++i)
		m_pTextureCom[i] = nullptr;

	for (_uint i = 0; i < (_uint)UI_LIFEBAR::LIFEBAR_END - 1; ++i)
		m_pTransformCom[i] = nullptr;
}

CUIBossLifeBar::~CUIBossLifeBar()
{
}

CUIBossLifeBar* CUIBossLifeBar::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIBossLifeBar* pUIBossLifeBar = new CUIBossLifeBar(_pGraphicDev);

	if (FAILED(pUIBossLifeBar->Ready_Unit()))
	{
		Safe_Release(pUIBossLifeBar);
		MSG_BOX("UIBossLifeBar Create Failed");
		return nullptr;
	}

	return pUIBossLifeBar;
}

HRESULT CUIBossLifeBar::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom[0]->Set_Pos(0.f, 160.f, 0.f);
	m_pTransformCom[1]->Set_Pos(-127.f, 160.f, 0.f);
	m_pTransformCom[2]->Set_Pos(0.f, 195.f, 0.f);

	m_pTransformCom[0]->Set_Scale(160.f, 160.f, 0.f);
	m_pTransformCom[1]->Set_Scale(20.f, 20.f, 0.f);
	m_pTransformCom[2]->Set_Scale(135.f, 135.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIBossLifeBar::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIBossLifeBar::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIBossLifeBar::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[0]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[1]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LOGO]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[2]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_BATTERY]->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIBossLifeBar::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_LifeBar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_LifeBar", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_A] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_LifeBar_A"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_LifeBar_A", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LOGO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Logo", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_BATTERY] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_Battery"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Battery", pComponent });

	pComponent = m_pTransformCom[0] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_LifeBar", pComponent });

	pComponent = m_pTransformCom[1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Logo", pComponent });

	pComponent = m_pTransformCom[2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Battery", pComponent });

	return S_OK;
}

void CUIBossLifeBar::Reset()
{
	m_pTransformCom[0]->Set_Pos(0.f, 160.f, 0.f);
	m_pTransformCom[1]->Set_Pos(-127.f, 160.f, 0.f);
	m_pTransformCom[2]->Set_Pos(0.f, 195.f, 0.f);

	m_pTransformCom[0]->Set_Scale(160.f, 160.f, 0.f);
	m_pTransformCom[1]->Set_Scale(20.f, 20.f, 0.f);
	m_pTransformCom[2]->Set_Scale(135.f, 135.f, 0.f);
}

void CUIBossLifeBar::Free()
{
	Engine::CUIUnit::Free();
}