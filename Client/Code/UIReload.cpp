#include "pch.h"
#include "..\Header\UIReload.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIReload::CUIReload(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_bReload(false)
{
	for (_uint i = 0; i < (_uint)UI_RELOAD::RELOAD_END; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUIReload::~CUIReload()
{
}

CUIReload* CUIReload::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIReload* pUIReload = new CUIReload(_pGraphicDev);

	if (FAILED(pUIReload->Ready_Unit()))
	{
		Safe_Release(pUIReload);
		MSG_BOX("UIReload create Failed");
		return nullptr;
	}

	return pUIReload;
}

HRESULT CUIReload::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_REDBOX]->Set_Pos(5.f, -75.f, 0.f);
	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_BLACKBOX]->Set_Pos(0.f, -70.f, 0.f);
	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_RGBBOX]->Set_Pos(40.f, -70.f, 0.f);
	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_R]->Set_Pos(-45.f, -70.f, 0.f);
	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_RELOAD]->Set_Pos(40.f, -70.f, 0.f);

	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_REDBOX]->Set_Scale(90.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_BLACKBOX]->Set_Scale(90.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_RGBBOX]->Set_Scale(50.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_R]->Set_Scale(13.f, 13.f, 0.f);
	m_pTransformCom[(_uint)UI_RELOAD::RELOAD_RELOAD]->Set_Scale(30.f, 30.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIReload::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIReload::LateUpdate_Unit()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));

	_float iMaxAmmo = (_float)(pPlayer->Get_MaxAmmo());
	_float iCurrAmmo = (_float)(pPlayer->Get_CurrAmmo());

	if (iCurrAmmo / iMaxAmmo <= 0.5f)
		m_bReload = true;
	else
		m_bReload = false;

	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIReload::Render_Unit()
{
	if (!m_bReload) return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_RELOAD::RELOAD_REDBOX]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_RELOAD::RELOAD_REDBOX]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_RELOAD::RELOAD_BLACKBOX]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_RELOAD::RELOAD_BLACKBOX]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_RELOAD::RELOAD_RGBBOX]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_RELOAD::RELOAD_RGBBOX]->Set_Texture();
		Update_Color();
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_RELOAD::RELOAD_R]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_RELOAD::RELOAD_R]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_RELOAD::RELOAD_RELOAD]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_RELOAD::RELOAD_RELOAD]->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIReload::Add_Component()
{
	CComponent* pComponent = NULL;

	{
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_RELOAD::RELOAD_REDBOX] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIReload_RedBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RedBox", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_RELOAD::RELOAD_BLACKBOX] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIReload_BlackBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_BlackBox", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_RELOAD::RELOAD_RGBBOX] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIReload_RGBBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RGBBox", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_RELOAD::RELOAD_R] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIReload_R"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_R", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_RELOAD::RELOAD_RELOAD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Reload"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Reload", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_RELOAD::RELOAD_REDBOX] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RedBox", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_RELOAD::RELOAD_BLACKBOX] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_BlackBox", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_RELOAD::RELOAD_RGBBOX] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RGBBox", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_RELOAD::RELOAD_R] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_R", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_RELOAD::RELOAD_RELOAD] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Reload", pComponent });
	}

	return S_OK;
}

void CUIReload::Update_Color()
{
	_float fHue = fmod(Engine::Get_Elapsed() * 70.f, 360.0f);
	_float fSaturation = 1.0f;
	_float fValue = 1.0f;

	m_pTextureCom[(_uint)UI_RELOAD::RELOAD_RGBBOX]->Change_TextureColor(fHue, fSaturation, fValue);
}

void CUIReload::Free()
{
	Engine::CUIUnit::Free();
}