#include "pch.h"
#include "..\Header\UIFloorBase.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIFloorBase::CUIFloorBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_iFloor(0)
	, m_fAlpha(0.f)
{
	for (_uint i = 0; i < (_uint)UI_FLOOR::FLOOR_END; i++)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUIFloorBase::~CUIFloorBase()
{
}

CUIFloorBase* CUIFloorBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIFloorBase* pUIFloorBase = new CUIFloorBase(_pGraphicDev);

	if (FAILED(pUIFloorBase->Ready_Unit()))
	{
		Safe_Release(pUIFloorBase);
		MSG_BOX("UIFloorBase create Failed");
		return nullptr;
	}

	return pUIFloorBase;
}

HRESULT CUIFloorBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Set_Pos(0.f, -100.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT1]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT2]->Set_Pos(0.f, 0.f, 0.f);

	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Set_Scale(100.f, 100.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Set_Scale(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Set_Scale(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Set_Scale(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Set_Scale(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Set_Scale(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT1]->Set_Scale(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT2]->Set_Scale(0.f, 0.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIFloorBase::Update_Unit(const _float& _fTimeDelta)
{
	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIFloorBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIFloorBase::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Set_Texture();
	Update_Color();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIFloorBase::Add_Component()
{
	CComponent* pComponent = NULL;

	{
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloor_BackGround"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_BackGround", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloor_Floor"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Word1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloor_Floor"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Word2", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD3] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloor_Floor"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Word3", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD4] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloor_Floor"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Word4", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD5] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloor_Floor"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Word5", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOOR::FLOOR_DIGIT1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_DWhite"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOOR::FLOOR_DIGIT2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_DWhite"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit2", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_BackGround", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Word1", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Word2", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Word3", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Word4", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Word5", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit1", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit2", pComponent });
	}

	return S_OK;
}

void CUIFloorBase::Update_Color()
{
	_float fHue = fmod(Engine::Get_Elapsed() * 40.f, 360.0f);
	_float fSaturation = 1.0f;
	_float fValue = 1.0f;

	m_pTextureCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Change_TextureColor(fHue, fSaturation, fValue);
}

void CUIFloorBase::Reset()
{
}

void CUIFloorBase::Free()
{
	Engine::CUIUnit::Free();
}