#include "pch.h"
#include "..\Header\UIEndingBase.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIEndingBase::CUIEndingBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CUIEndingBase::~CUIEndingBase()
{
}

CUIEndingBase* CUIEndingBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIEndingBase* pUIEndingBase = new CUIEndingBase(_pGraphicDev);

	if (FAILED(pUIEndingBase->Ready_Unit()))
	{
		Safe_Release(pUIEndingBase);
		MSG_BOX("UIEndingBase create Failed");
		return nullptr;
	}

	return pUIEndingBase;
}

HRESULT CUIEndingBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bRender = true;

	return S_OK;
}

_int CUIEndingBase::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIEndingBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIEndingBase::Render_Unit()
{
}

HRESULT CUIEndingBase::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UISkill_Back"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Back", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Back", pComponent });

	return S_OK;
}

void CUIEndingBase::Free()
{
	Engine::CUIUnit::Free();
}