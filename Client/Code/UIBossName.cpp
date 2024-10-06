#include "pch.h"
#include "..\Header\UIBossName.h"
#include "Export_Utility.h"

CUIBossName::CUIBossName(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CUIBossName::~CUIBossName()
{
}

CUIBossName* CUIBossName::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIBossName* pUIBossName = new CUIBossName(_pGraphicDev);

	if (FAILED(pUIBossName->Ready_Unit()))
	{
		Safe_Release(pUIBossName);
		MSG_BOX("UIBossName Create Failed");
		return nullptr;
	}

	return pUIBossName;
}

HRESULT CUIBossName::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(0.f, 135.f, 0.f);

	m_pTransformCom->Set_Scale(110.f, 110.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIBossName::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIBossName::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIBossName::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIBossName::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Roboto"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Roboto", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Roboto", pComponent });

	return S_OK;
}

void CUIBossName::Reset()
{
	m_pTransformCom->Set_Pos(0.f, 135.f, 0.f);

	m_pTransformCom->Set_Scale(110.f, 110.f, 0.f);
}

void CUIBossName::Free()
{
	Engine::CUIUnit::Free();
}