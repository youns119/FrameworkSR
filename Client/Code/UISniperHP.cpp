#include "pch.h"
#include "..\Header\UISniperHP.h"
#include "Export_Utility.h"

CUISniperHP::CUISniperHP(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_fHP(0.f)
	, m_fScaleRatio(0.f)
{
	for (_uint i = 0; i < (_uint)UI_SNIPERHP::SNIPERHP_END; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUISniperHP::~CUISniperHP()
{
}

CUISniperHP* CUISniperHP::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUISniperHP* pUISniperHP = new CUISniperHP(_pGraphicDev);

	if (FAILED(pUISniperHP->Ready_Unit()))
	{
		Safe_Release(pUISniperHP);
		MSG_BOX("UISniperHP Create Failed");
		return nullptr;
	}

	return pUISniperHP;
}

HRESULT CUISniperHP::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fHP = 0;
	m_fScaleRatio = 0.f;

	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX1]->Set_Pos(0.f, WINCY / 2.f - 40.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX2]->Set_Pos(0.f, WINCY / 2.f - 115.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BAR]->Set_Pos(0.f, WINCY / 2.f - 40.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT1]->Set_Pos(-43.f, WINCY / 2.f - 115.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT2]->Set_Pos(0.f, WINCY / 2.f - 115.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT3]->Set_Pos(43.f, WINCY / 2.f - 115.f, 0.f);

	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX1]->Set_Scale(150.f, 50.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX2]->Set_Scale(150.f, 130.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BAR]->Set_Scale(130.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT1]->Set_Scale(32.f, 32.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT2]->Set_Scale(32.f, 32.f, 0.f);
	m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT3]->Set_Scale(32.f, 32.f, 0.f);

	m_bRender = false;

	return S_OK;
}

_int CUISniperHP::Update_Unit(const _float& _fTimeDelta)
{
	m_fScaleRatio = m_fHP / 99.f;

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUISniperHP::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUISniperHP::Render_Unit()
{
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX1]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX1]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX2]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX2]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BAR]->Set_Scale(130.f * m_fScaleRatio, 15.f, 0.f);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BAR]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_BAR]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}

	{
		if ((int)m_fHP / 10 != 0)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT1]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT1]->Set_Texture((int)(m_fHP / 10.f));
			m_pBufferCom->Render_Buffer();
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT2]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT2]->Set_Texture((int)m_fHP % 10);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT3]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT3]->Set_Texture(10);
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUISniperHP::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UISniperHP_Box"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Box1", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UISniperHP_Box"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Box2", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_BAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UISniperHP_Bar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Bar", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DRed"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit1", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DRed"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit2", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT3] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DRed"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit3", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Box1", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BOX2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Box2", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_BAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Bar", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit1", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit2", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_SNIPERHP::SNIPERHP_DIGIT3] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit3", pComponent });

	return S_OK;
}

void CUISniperHP::Reset()
{
	m_bRender = false;

	m_fHP = 0.f;
	m_fScaleRatio = 0.f;
}

void CUISniperHP::Free()
{
	Engine::CUIUnit::Free();
}