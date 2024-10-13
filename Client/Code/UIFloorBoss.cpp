#include "pch.h"
#include "..\Header\UIFloorBoss.h"
#include "Export_Utility.h"

CUIFloorBoss::CUIFloorBoss(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
{
	for (_uint i = 0; i < (_uint)UI_FLOORBOSS::FLOORBOSS_END; i++)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUIFloorBoss::~CUIFloorBoss()
{
}

CUIFloorBoss* CUIFloorBoss::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIFloorBoss* pUIFloorBoss = new CUIFloorBoss(_pGraphicDev);

	if (FAILED(pUIFloorBoss->Ready_Unit()))
	{
		Safe_Release(pUIFloorBoss);
		MSG_BOX("UIFloorBoss create Failed");
		return nullptr;
	}

	return pUIFloorBoss;
}

HRESULT CUIFloorBoss::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER]->Set_Pos(0.f, 0.f, 0.f);

	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER]->Set_Scale(200.f, 200.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIFloorBoss::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIFloorBoss::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIFloorBoss::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER]->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIFloorBoss::Add_Component()
{
	CComponent* pComponent = NULL;

	{
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Danger"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Danger", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Bar"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Bar", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_NETWORK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Network"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Network", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIMELIMIT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Timelimit"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Timelimit", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIME] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Zero"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Time", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Danger", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Bar", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_NETWORK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Network", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIMELIMIT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Timelimit", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIME] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Time", pComponent });
	}

	return S_OK;
}

void CUIFloorBoss::Reset()
{
}

void CUIFloorBoss::Free()
{
	Engine::CUIUnit::Free();
}