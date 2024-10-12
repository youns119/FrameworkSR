#include "pch.h"
#include "..\Header\UIFloorBase.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIFloorBase::CUIFloorBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_iFloor(0)
	, m_fTimeDelay(0.f)
	, m_fAlpha(0.f)
{
	for (_uint i = 0; i < (_uint)UI_FLOOR::FLOOR_END; i++)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}

	for (_int i = 0; i < 5; i++)
	{
		m_fScaleRatio[i] = 0.f;
		m_fPosRatio[i] = 0.f;
		m_vDir[i] = { 0.f, 0.f, 0.f };
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

	for (_int i = 0; i < 5; i++)
	{
		m_fScaleRatio[i] = -1.f;
		m_fPosRatio[i] = 1.f;
	}

	m_vDir[0] = { -1.f * cosf(D3DXToRadian(30.f)), 1.f * sinf(D3DXToRadian(30.f)), 0.f };
	m_vDir[1] = { -1.f * cosf(D3DXToRadian(60.f)), 1.f * sinf(D3DXToRadian(60.f)), 0.f };
	m_vDir[2] = { 0.f, 1.f, 0.f };
	m_vDir[3] = { 1.f * cosf(D3DXToRadian(60.f)), 1.f * sinf(D3DXToRadian(60.f)), 0.f };
	m_vDir[4] = { 1.f * cosf(D3DXToRadian(30.f)), 1.f * sinf(D3DXToRadian(30.f)), 0.f };

	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Set_Pos(0.f, WINCY / 2.f - 200.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Set_Pos(-130.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Set_Pos(-65.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Set_Pos(0.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Set_Pos(65.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Set_Pos(130.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT1]->Set_Pos(-45.f, 70.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT2]->Set_Pos(45.f, 70.f, 0.f);

	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Set_Scale(180.f, 180.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT1]->Set_Scale(60.f, 60.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT2]->Set_Scale(60.f, 60.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIFloorBase::Update_Unit(const _float& _fTimeDelta)
{
	m_fTimeDelay += _fTimeDelta;

	if (m_fTimeDelay >= 2.f)
	{
		m_pOwnerUI->Set_Render(false);
		return 0;
	}

	m_iFloor = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Get_CurrFloor();

	{
		if (m_fTimeDelay < 1.f)
		{
			if (m_fTimeDelay >= 0.1f)
			{
				m_fScaleRatio[0] += _fTimeDelta * 5.f;
				if (m_fScaleRatio[0] >= 1.f) m_fScaleRatio[0] = 1.f;
				m_fPosRatio[0] -= _fTimeDelta * 3.f;
				if (m_fPosRatio[0] <= 0.f) m_fPosRatio[0] = 0.f;

				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Set_Scale(40.f * m_fScaleRatio[0], 40.f, 0.f);
				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Set_Pos(-130.f - 30.f * m_fPosRatio[0], WINCY / 2.f - 145.f + 30.f * m_fPosRatio[0], 0.f);
			}

			if (m_fTimeDelay >= 0.2f)
			{
				m_fScaleRatio[1] += _fTimeDelta * 5.f;
				if (m_fScaleRatio[1] >= 1.f) m_fScaleRatio[1] = 1.f;
				m_fPosRatio[1] -= _fTimeDelta * 5.f;
				if (m_fPosRatio[1] <= 0.f) m_fPosRatio[1] = 0.f;

				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Set_Scale(40.f * m_fScaleRatio[1], 40.f, 0.f);
				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Set_Pos(-65.f - 30.f * m_fPosRatio[1], WINCY / 2.f - 145.f + 30.f * m_fPosRatio[1], 0.f);
			}

			if (m_fTimeDelay >= 0.3f)
			{
				m_fScaleRatio[2] += _fTimeDelta * 5.f;
				if (m_fScaleRatio[2] >= 1.f) m_fScaleRatio[2] = 1.f;
				m_fPosRatio[2] -= _fTimeDelta * 5.f;
				if (m_fPosRatio[2] <= 0.f) m_fPosRatio[2] = 0.f;

				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Set_Scale(40.f * m_fScaleRatio[2], 40.f, 0.f);
				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Set_Pos(-30.f * m_fPosRatio[2], WINCY / 2.f - 145.f + 30.f * m_fPosRatio[2], 0.f);
			}

			if (m_fTimeDelay >= 0.4f)
			{
				m_fScaleRatio[3] += _fTimeDelta * 5.f;
				if (m_fScaleRatio[3] >= 1.f) m_fScaleRatio[3] = 1.f;
				m_fPosRatio[3] -= _fTimeDelta * 5.f;
				if (m_fPosRatio[3] <= 0.f) m_fPosRatio[3] = 0.f;

				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Set_Scale(40.f * m_fScaleRatio[3], 40.f, 0.f);
				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Set_Pos(65.f - 30.f * m_fPosRatio[3], WINCY / 2.f - 145.f + 30.f * m_fPosRatio[3], 0.f);
			}

			if (m_fTimeDelay >= 0.5f)
			{
				m_fScaleRatio[4] += _fTimeDelta * 5.f;
				if (m_fScaleRatio[4] >= 1.f) m_fScaleRatio[4] = 1.f;
				m_fPosRatio[4] -= _fTimeDelta * 5.f;
				if (m_fPosRatio[4] <= 0.f) m_fPosRatio[4] = 0.f;

				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Set_Scale(40.f * m_fScaleRatio[4], 40.f, 0.f);
				m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Set_Pos(130.f - 30.f * m_fPosRatio[4], WINCY / 2.f - 145.f + 30.f * m_fPosRatio[4], 0.f);
			}
		}
		else
		{
			m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Move_Pos(&m_vDir[0], _fTimeDelta, 450.f);
			m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Move_Pos(&m_vDir[1], _fTimeDelta, 450.f);
			m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Move_Pos(&m_vDir[2], _fTimeDelta, 450.f);
			m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Move_Pos(&m_vDir[3], _fTimeDelta, 450.f);
			m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Move_Pos(&m_vDir[4], _fTimeDelta, 450.f);
		}
	}

	if (m_fAlpha < 1.f)
	{
		m_fAlpha += _fTimeDelta;
		if (m_fAlpha >= 1.f) m_fAlpha = 1.f;
	}

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIFloorBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIFloorBase::Render_Unit()
{
	{
		DWORD tPreFactor;

		m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Change_Alpha(m_fAlpha);
		m_pTextureCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Set_Texture();
		Update_Color();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
	}

	if (m_fTimeDelay >= 0.1f)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Set_Texture(0);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fTimeDelay >= 0.2f)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Set_Texture(1);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fTimeDelay >= 0.3f)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Set_Texture(2);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fTimeDelay >= 0.4f)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Set_Texture(3);
		m_pBufferCom->Render_Buffer();
	}

	if (m_fTimeDelay >= 0.5f)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Set_Texture(4);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT1]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_FLOOR::FLOOR_DIGIT1]->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT2]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_FLOOR::FLOOR_DIGIT2]->Set_Texture(m_iFloor);
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
	m_iFloor = 0;
	m_fTimeDelay = 0.f;
	m_fAlpha = 0.f;

	for (_int i = 0; i < 5; i++)
	{
		m_fScaleRatio[i] = -1.f;
		m_fPosRatio[i] = 1.f;
	}

	m_vDir[0] = { -1.f * cosf(D3DXToRadian(30.f)), 1.f * sinf(D3DXToRadian(30.f)), 0.f };
	m_vDir[1] = { -1.f * cosf(D3DXToRadian(60.f)), 1.f * sinf(D3DXToRadian(60.f)), 0.f };
	m_vDir[2] = { 0.f, 1.f, 0.f };
	m_vDir[3] = { 1.f * cosf(D3DXToRadian(60.f)), 1.f * sinf(D3DXToRadian(60.f)), 0.f };
	m_vDir[4] = { 1.f * cosf(D3DXToRadian(30.f)), 1.f * sinf(D3DXToRadian(30.f)), 0.f };

	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Set_Pos(0.f, WINCY / 2.f - 200.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Set_Pos(-130.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Set_Pos(-65.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Set_Pos(0.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Set_Pos(65.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Set_Pos(130.f, WINCY / 2.f - 145.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT1]->Set_Pos(-45.f, 70.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT2]->Set_Pos(45.f, 70.f, 0.f);

	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_BACKGROUND]->Set_Scale(180.f, 180.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD1]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD2]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD3]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD4]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_WORD5]->Set_Scale(40.f, 40.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT1]->Set_Scale(60.f, 60.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOOR::FLOOR_DIGIT2]->Set_Scale(60.f, 60.f, 0.f);
}

void CUIFloorBase::Free()
{
	Engine::CUIUnit::Free();
}