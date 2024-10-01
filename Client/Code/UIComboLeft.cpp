#include "pch.h"
#include "..\Header\UIComboLeft.h"
#include "Export_Utility.h"

CUIComboLeft::CUIComboLeft(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_fScaleRatio(0.f)
	, m_iDigit(0)
{
	for (_uint i = 0; i < (_uint)UI_COMBOLEFT::COMBOLEFT_END; ++i)
		m_pTextureCom[i] = nullptr;

	for (int i = 0; i < 2; ++i)
		m_pTransformCom[i] = nullptr;
}

CUIComboLeft::~CUIComboLeft()
{
}

CUIComboLeft* CUIComboLeft::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIComboLeft* pUIComboLeft = new CUIComboLeft(_pGraphicDev);

	if (FAILED(pUIComboLeft->Ready_Unit()))
	{
		Safe_Release(pUIComboLeft);
		MSG_BOX("UIComboLeft Create Failed");
		return nullptr;
	}

	return pUIComboLeft;
}

HRESULT CUIComboLeft::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bRender = true;

	return S_OK;
}

_int CUIComboLeft::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIComboLeft::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIComboLeft::Render_Unit()
{

}

HRESULT CUIComboLeft::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_COMBOLEFT::COMBOLEFT_BACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Back", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_COMBOLEFT::COMBOLEFT_X] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_X"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_X", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_COMBOLEFT::COMBOLEFT_DIGIT1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_Digit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit1", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_COMBOLEFT::COMBOLEFT_DIGIT2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UI_Digit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit2", pComponent });

	pComponent = m_pTransformCom[0] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Back", pComponent });

	pComponent = m_pTransformCom[1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit", pComponent });

	return S_OK;
}

void CUIComboLeft::Free()
{
	Engine::CUIUnit::Free();
}