#include "pch.h"
#include "..\Header\UIActionBase.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIActionBase::CUIActionBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_bTick(false)
	, m_iActionIndex(0)
	, m_fColor(0.f)
	, m_fTick(0.f)
	, m_fSecond(0.f)
{
	for (_uint i = 0; i < (_uint)UI_ACTION::ACTION_END; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}

	for (_int i = 0; i < 2; i++)
		m_fScaleRatio[i] = 0.f;
}

CUIActionBase::~CUIActionBase()
{
}

CUIActionBase* CUIActionBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIActionBase* pUIActionBase = new CUIActionBase(_pGraphicDev);

	if (FAILED(pUIActionBase->Ready_Unit()))
	{
		Safe_Release(pUIActionBase);
		MSG_BOX("UIActionBase create Failed");
		return nullptr;
	}

	return pUIActionBase;
}

HRESULT CUIActionBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	for (_int i = 0; i < 2; i++)
		m_fScaleRatio[i] = 5.f;

	m_pTransformCom[(_uint)UI_ACTION::ACTION_RGB]->Set_Pos(0.f, WINCY / 2.f - 140.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_OUTLINE]->Set_Pos(0.f, WINCY / 2.f - 210.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_INNER]->Set_Pos(0.f, WINCY / 2.f - 210.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_ICON]->Set_Pos(0.f, WINCY / 2.f - 210.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_TEXT]->Set_Pos(0.f, WINCY / 2.f - 140.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_SECOND]->Set_Pos(0.f, WINCY / 2.f - 180.f, 0.f);

	m_pTransformCom[(_uint)UI_ACTION::ACTION_RGB]->Set_Scale(200.f, 200.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_OUTLINE]->Set_Scale(50.f, 50.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_INNER]->Set_Scale(47.f, 47.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_ICON]->Set_Scale(60.f, 60.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_TEXT]->Set_Scale(130.f, 130.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_SECOND]->Set_Scale(110.f, 110.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIActionBase::Update_Unit(const _float& _fTimeDelta)
{
	m_fColor = 0.f;
	m_fSecond += _fTimeDelta;

	if (m_fScaleRatio[0] > 1.f)
	{
		m_fScaleRatio[0] -= _fTimeDelta * 15.f;
		if (m_fScaleRatio[0] <= 1.f) m_fScaleRatio[0] = 1.f;

		m_pTransformCom[(_uint)UI_ACTION::ACTION_TEXT]->Set_Scale(130.f * m_fScaleRatio[0], 130.f * m_fScaleRatio[0], 0.f);
	}

	if (m_fSecond >= 0.7f)
	{
		if (m_fScaleRatio[1] > 1.f)
		{
			m_fScaleRatio[1] -= _fTimeDelta * 15.f;
			if (m_fScaleRatio[1] <= 1.f) m_fScaleRatio[1] = 1.f;

			m_pTransformCom[(_uint)UI_ACTION::ACTION_SECOND]->Set_Scale(110.f * m_fScaleRatio[1], 110.f * m_fScaleRatio[1], 0.f);
		}
	}

	{
		m_fTick += _fTimeDelta;

		if (m_fTick >= 1.f) m_fTick = 0.f;

		if (m_fTick <= 0.5f) m_bTick = false;
		else if (m_fTick > 0.5f) m_bTick = true;
	}

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIActionBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIActionBase::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ACTION::ACTION_RGB]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ACTION::ACTION_RGB]->Set_Texture();
	Update_Color(m_pTextureCom[(_uint)UI_ACTION::ACTION_RGB]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ACTION::ACTION_OUTLINE]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ACTION::ACTION_OUTLINE]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ACTION::ACTION_INNER]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ACTION::ACTION_INNER]->Set_Texture();
	Update_Color(m_pTextureCom[(_uint)UI_ACTION::ACTION_INNER]);
	m_pBufferCom->Render_Buffer();

	if (!m_bTick)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ACTION::ACTION_ICON]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_ACTION::ACTION_ICON]->Set_Texture(m_iActionIndex);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ACTION::ACTION_TEXT]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ACTION::ACTION_TEXT]->Set_Texture(m_iActionIndex);
	m_pBufferCom->Render_Buffer();

	if (m_fSecond >= 0.7f)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ACTION::ACTION_SECOND]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_ACTION::ACTION_SECOND]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUIActionBase::Add_Component()
{
	CComponent* pComponent = NULL;

	{
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ACTION::ACTION_RGB] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIAction_BackGround"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RGB", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ACTION::ACTION_OUTLINE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIAction_Outline"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Outline", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ACTION::ACTION_INNER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIAction_Inner"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Inner", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ACTION::ACTION_ICON] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIAction_Icon"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Icon", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ACTION::ACTION_TEXT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIAction_Text"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Text", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ACTION::ACTION_SECOND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIAction_Second"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Second", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_ACTION::ACTION_RGB] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RGB", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ACTION::ACTION_OUTLINE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Outline", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ACTION::ACTION_INNER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Inner", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ACTION::ACTION_ICON] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Icon", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ACTION::ACTION_TEXT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Text", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ACTION::ACTION_SECOND] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Second", pComponent });
	}

	return S_OK;
}

void CUIActionBase::Update_Color(CTexture* _pTexture)
{
	m_fColor += 100.f;

	_float fHue = fmod(Engine::Get_Elapsed() * m_fColor, 360.0f);
	_float fSaturation = 1.0f;
	_float fValue = 1.0f;

	_pTexture->Change_TextureColor(fHue, fSaturation, fValue);
}

void CUIActionBase::Reset()
{
	m_bTick = false;
	m_iActionIndex = 0;
	m_fColor = 0.f;
	m_fTick = 0.f;
	m_fSecond = 0.f;

	for (_int i = 0; i < 2; i++)
		m_fScaleRatio[i] = 5.f;

	m_pTransformCom[(_uint)UI_ACTION::ACTION_RGB]->Set_Pos(0.f, WINCY / 2.f - 140.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_OUTLINE]->Set_Pos(0.f, WINCY / 2.f - 210.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_INNER]->Set_Pos(0.f, WINCY / 2.f - 210.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_ICON]->Set_Pos(0.f, WINCY / 2.f - 210.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_TEXT]->Set_Pos(0.f, WINCY / 2.f - 140.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_SECOND]->Set_Pos(0.f, WINCY / 2.f - 180.f, 0.f);

	m_pTransformCom[(_uint)UI_ACTION::ACTION_RGB]->Set_Scale(200.f, 200.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_OUTLINE]->Set_Scale(50.f, 50.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_INNER]->Set_Scale(47.f, 47.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_ICON]->Set_Scale(60.f, 60.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_TEXT]->Set_Scale(130.f, 130.f, 0.f);
	m_pTransformCom[(_uint)UI_ACTION::ACTION_SECOND]->Set_Scale(110.f, 110.f, 0.f);

	m_bRender = true;
}

void CUIActionBase::Free()
{
	Engine::CUIUnit::Free();
}