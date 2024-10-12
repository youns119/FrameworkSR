#include "pch.h"
#include "..\Header\UIComboBase.h"
#include "Export_Utility.h"

CUIComboBase::CUIComboBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformTexture(nullptr)
	, m_pTexture(nullptr)
	, m_pTextureSurface(nullptr)
	, m_pPreSurface(nullptr)
	, m_bTick(false)
	, m_iCombo(1)
	, m_fTimeDelay(0.f)
	, m_fTickTime(0.f)
	, m_fDuration(0.f)
{
	for (_uint i = 0; i < (_uint)UI_COMBO::COMBO_END; i++)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}

	for (_int i = 0; i < 2; i++)
	{
		m_iRightDigit[i] = 0;
		m_iLeftDigit[i] = 0;
		m_fScaleRatio[i] = 0.f;
	}
}

CUIComboBase::~CUIComboBase()
{
}

CUIComboBase* CUIComboBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIComboBase* pUIComboBase = new CUIComboBase(_pGraphicDev);

	if (FAILED(pUIComboBase->Ready_Unit()))
	{
		Safe_Release(pUIComboBase);
		MSG_BOX("UIComboBase Create Failed");
		return nullptr;
	}

	return pUIComboBase;
}

HRESULT CUIComboBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	for (_int i = 0; i < 2; i++)
		m_fScaleRatio[i] = 3.f;

	m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTBACK]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_COMBO]->Set_Pos(12.f, 16.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_PLUS]->Set_Pos(-58.f, -18.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT1]->Set_Pos(-25.f, -18.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT2]->Set_Pos(-5.f, -18.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_SEC]->Set_Pos(30.f, -22.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTBACK]->Set_Pos(-150.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_X]->Set_Pos(-180.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT1]->Set_Pos(-140.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT2]->Set_Pos(-110.f, 0.f, 0.f);
	m_pTransformTexture->Set_Pos(-WINCX / 2.f + 230.f, -20.f, 0.f);
	m_pTransformTexture->Set_Angle(0.f, 0.f, D3DXToRadian(5.f));

	m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTBACK]->Set_Scale(75.f, 35.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_COMBO]->Set_Scale(65.f, 65.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_PLUS]->Set_Scale(13.f, 13.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT1]->Set_Scale(15.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT2]->Set_Scale(15.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_SEC]->Set_Scale(20.f, 20.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTBACK]->Set_Scale(70.f, 35.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_X]->Set_Scale(30.f, 30.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT1]->Set_Scale(30.f, 30.f, 0.f);
	m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT2]->Set_Scale(30.f, 30.f, 0.f);
	m_pTransformTexture->Set_Scale(WINCX / 2.f * 0.9f, WINCY / 2.f * 0.9f, 0.f);

	m_pGraphicDev->CreateTexture
	(
		WINCX, WINCY,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexture,
		NULL
	);

	m_pTexture->GetSurfaceLevel(0, &m_pTextureSurface);
	m_pGraphicDev->GetRenderTarget(0, &m_pPreSurface);

	m_bRender = true;

	return S_OK;
}

_int CUIComboBase::Update_Unit(const _float& _fTimeDelta)
{
	if (m_fDuration >= 3.5f)
	{
		m_pOwnerUI->Set_Render(false);
		return 0;
	}

	if (m_iCombo >= 2)
	{
		m_fTimeDelay += _fTimeDelta;
		m_fDuration += _fTimeDelta;

		if (m_fDuration >= 3.f)
		{
			m_fTickTime += _fTimeDelta;

			if (m_fTickTime >= 0.05f)
			{
				m_bTick = !m_bTick;
				m_fTickTime = 0.f;
			}
		}

		if (m_fTimeDelay >= 0.2f)
		{
			if (m_fScaleRatio[0] > 1.f)
			{
				m_fScaleRatio[0] -= _fTimeDelta * 7.f;
				if (m_fScaleRatio[0] <= 1.f) m_fScaleRatio[0] = 1.f;
			}
		}

		if (m_fTimeDelay >= 0.4f)
		{
			if (m_fScaleRatio[1] > 1.f)
			{
				m_fScaleRatio[1] -= _fTimeDelta * 7.f;
				if (m_fScaleRatio[1] <= 1.f) m_fScaleRatio[1] = 1.f;
			}
		}

		m_pTransformCom[(_uint)UI_COMBO::COMBO_PLUS]->Set_Scale(13.f * m_fScaleRatio[0], 13.f * m_fScaleRatio[0], 0.f);
		m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT1]->Set_Scale(15.f * m_fScaleRatio[0], 15.f * m_fScaleRatio[0], 0.f);
		m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT2]->Set_Scale(15.f * m_fScaleRatio[0], 15.f * m_fScaleRatio[0], 0.f);
		m_pTransformCom[(_uint)UI_COMBO::COMBO_SEC]->Set_Scale(20.f * m_fScaleRatio[0], 20.f * m_fScaleRatio[0], 0.f);

		m_pTransformCom[(_uint)UI_COMBO::COMBO_COMBO]->Set_Scale(65.f * m_fScaleRatio[1], 65.f * m_fScaleRatio[1], 0.f);
		m_pTransformCom[(_uint)UI_COMBO::COMBO_X]->Set_Scale(30.f * m_fScaleRatio[1], 30.f * m_fScaleRatio[1], 0.f);
		m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT1]->Set_Scale(30.f * m_fScaleRatio[1], 30.f * m_fScaleRatio[1], 0.f);
		m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT2]->Set_Scale(30.f * m_fScaleRatio[1], 30.f * m_fScaleRatio[1], 0.f);
	}

	Update_Combo();

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIComboBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIComboBase::Render_Unit()
{
	if (m_iCombo < 2 || m_bTick) return;

	{
		m_pGraphicDev->GetRenderTarget(0, &m_pPreSurface);
		m_pGraphicDev->SetRenderTarget(0, m_pTextureSurface);
		m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTBACK]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_COMBO::COMBO_RIGHTBACK]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		if (m_fTimeDelay >= 0.4f)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_COMBO]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_COMBO::COMBO_COMBO]->Set_Texture();
			m_pBufferCom->Render_Buffer();
		}

		if (m_fTimeDelay >= 0.2f)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_PLUS]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_COMBO::COMBO_PLUS]->Set_Texture();
			m_pBufferCom->Render_Buffer();
		}

		if (m_fTimeDelay >= 0.4f)
		{
			if (m_iRightDigit[0] != 0)
			{
				m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT1]->Get_WorldMatrix());

				m_pTextureCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT1]->Set_Texture(m_iRightDigit[0]);
				m_pBufferCom->Render_Buffer();
			}
		}

		if (m_fTimeDelay >= 0.2f)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT2]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT2]->Set_Texture(m_iRightDigit[1]);
			m_pBufferCom->Render_Buffer();
		}

		if (m_fTimeDelay >= 0.2f)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_SEC]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_COMBO::COMBO_SEC]->Set_Texture();
			m_pBufferCom->Render_Buffer();
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTBACK]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_COMBO::COMBO_LEFTBACK]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		if (m_fTimeDelay >= 0.4f)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_X]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_COMBO::COMBO_X]->Set_Texture();
			m_pBufferCom->Render_Buffer();
		}

		if (m_fTimeDelay >= 0.4f)
		{
			if (m_iLeftDigit[0] != 0)
			{
				m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT1]->Get_WorldMatrix());

				m_pTextureCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT1]->Set_Texture(m_iLeftDigit[0]);
				m_pBufferCom->Render_Buffer();
			}
		}

		if (m_fTimeDelay >= 0.4f)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT2]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT2]->Set_Texture(m_iLeftDigit[1]);
			m_pBufferCom->Render_Buffer();
		}

		{
			m_pGraphicDev->SetRenderTarget(0, m_pPreSurface);

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformTexture->Get_WorldMatrix());

			m_pGraphicDev->SetTexture(0, m_pTexture);
			m_pBufferCom->Render_Buffer();
		}
	}
}

HRESULT CUIComboBase::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	{
		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_RIGHTBACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Bar"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RightBack", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_COMBO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_Combo"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Combo", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_PLUS] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_Like"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Plus", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_White"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RightDigit1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_White"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RightDigit2", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_SEC] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_Sec"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Sec", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_LEFTBACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Bar"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_LeftBack", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_X] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_DWhite_X"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_X", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_DWhite"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_LeftDigit1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UICombo_DWhite"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_LeftDigit2", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTBACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RightBack", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_COMBO] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Combo", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_PLUS] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Plus", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RightDigit1", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_RIGHTDIGIT2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RightDigit2", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_SEC] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Sec", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTBACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_LeftBack", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_X] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_X", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_LeftDigit1", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_COMBO::COMBO_LEFTDIGIT2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_LeftDigit2", pComponent });

		pComponent = m_pTransformTexture = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Texture", pComponent });
	}

	return S_OK;
}

void CUIComboBase::Update_Combo()
{
	m_iRightDigit[0] = (m_iCombo - 1) / 10;
	m_iRightDigit[1] = (m_iCombo - 1) % 10;

	m_iLeftDigit[0] = m_iCombo / 10;
	m_iLeftDigit[1] = m_iCombo % 10;
}

void CUIComboBase::Reset()
{
	m_bTick = false;
	m_iCombo = 1;
	m_fTimeDelay = 0.f;
	m_fTickTime = 0.f;
	m_fDuration = 0.f;

	for (_int i = 0; i < 2; i++)
	{
		m_iRightDigit[i] = 0;
		m_iLeftDigit[i] = 0;
		m_fScaleRatio[i] = 3.f;
	}
}

void CUIComboBase::Free()
{
	m_pTextureSurface->Release();
	m_pPreSurface->Release();
	m_pTexture->Release();

	Engine::CUIUnit::Free();
}