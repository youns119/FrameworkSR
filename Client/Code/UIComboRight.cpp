#include "pch.h"
#include "..\Header\UIComboRight.h"
#include "Export_Utility.h"

CUIComboRight::CUIComboRight(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_fScaleRatio(0.f)
{
	for (_uint i = 0; i < (_uint)UI_COMBORIGHT::COMBORIGHT_END; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUIComboRight::~CUIComboRight()
{
}

CUIComboRight* CUIComboRight::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIComboRight* pUIComboRight = new CUIComboRight(_pGraphicDev);

	if (FAILED(pUIComboRight->Ready_Unit()))
	{
		Safe_Release(pUIComboRight);
		MSG_BOX("UIComboRight Create Failed");
		return nullptr;
	}

	return pUIComboRight;
}

HRESULT CUIComboRight::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bRender = true;

	return S_OK;
}

_int CUIComboRight::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIComboRight::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIComboRight::Render_Unit()
{
}

HRESULT CUIComboRight::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_COMBORIGHT::COMBORIGHT_BACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Back", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_COMBORIGHT::COMBORIGHT_COMBO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_COMBO"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Text_COMBO", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_COMBORIGHT::COMBORIGHT_SEC] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPlus_White"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Plus", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_COMBORIGHT::COMBORIGHT_BACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Back", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_COMBORIGHT::COMBORIGHT_COMBO] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Text_Combo", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_COMBORIGHT::COMBORIGHT_SEC] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Plus", pComponent });

	return S_OK;
}

void CUIComboRight::Free()
{
	Engine::CUIUnit::Free();
}