#include "pch.h"
#include "..\Header\UIItem.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIItem::CUIItem(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_bTick(false)
	, m_fColor(0.f)
	, m_fScaleRatio(0.f)
	, m_fTick(0.f)
{
	for (_uint i = 0; i < (_uint)UI_ITEM::ITEM_END; i++)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUIItem::~CUIItem()
{
}

CUIItem* CUIItem::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIItem* pUIItem = new CUIItem(_pGraphicDev);

	if (FAILED(pUIItem->Ready_Unit()))
	{
		Safe_Release(pUIItem);
		MSG_BOX("UIItem create Failed");
		return nullptr;
	}

	return pUIItem;
}

HRESULT CUIItem::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fScaleRatio = 4.f;

	m_pTransformCom[(_uint)UI_ITEM::ITEM_OUTLINE]->Set_Pos(0.f, -150.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_INNER]->Set_Pos(0.f, -150.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_RGBBOX]->Set_Pos(0.f, -230.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_TEXTBOX]->Set_Pos(0.f, -218.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_EXECUTION]->Set_Pos(0.f, -245.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_MOUSE]->Set_Pos(0.f, -218.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_ICON]->Set_Pos(0.f, -WINCY / 2.f - 100.f, 0.f);

	m_pTransformCom[(_uint)UI_ITEM::ITEM_OUTLINE]->Set_Scale(57.f, 57.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_INNER]->Set_Scale(55.f, 55.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_RGBBOX]->Set_Scale(80.f, 30.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_TEXTBOX]->Set_Scale(77.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_EXECUTION]->Set_Scale(25.f, 25.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_MOUSE]->Set_Scale(50.f, 50.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_ICON]->Set_Scale(70.f, 70.f, 0.f);

	m_bRender = false;

	return S_OK;
}

_int CUIItem::Update_Unit(const _float& _fTimeDelta)
{
	m_fColor = 0.f;

	_vec3 vCurrPos;

	m_pTransformCom[(_uint)UI_ITEM::ITEM_ICON]->Get_Info(INFO::INFO_POS, &vCurrPos);

	if (vCurrPos.y < -150.f)
	{
		_float fY = vCurrPos.y + (-150.f - vCurrPos.y) * _fTimeDelta * 7.f;

		if (fabs(-150.f - fY) <= 0.1f)
			fY = -150.f;

		m_pTransformCom[(_uint)UI_ITEM::ITEM_ICON]->Set_Pos(0.f, fY, 0.f);
	}

	if (vCurrPos.y >= -150.f)
		if (m_fScaleRatio > 1.f)
		{
			m_fScaleRatio -= _fTimeDelta * 10.f;
			if (m_fScaleRatio <= 1.f) m_fScaleRatio = 1.f;
		}

	m_pTransformCom[(_uint)UI_ITEM::ITEM_ICON]->Set_Scale(70.f * m_fScaleRatio, 70.f * m_fScaleRatio, 0.f);

	if (m_fScaleRatio <= 1.f)
	{
		m_fTick += _fTimeDelta;

		if (m_fTick >= 1.f) m_fTick = 0.f;

		if (m_fTick <= 0.5f) m_bTick = false;
		else if (m_fTick > 0.5f) m_bTick = true;
	}

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIItem::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIItem::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ITEM::ITEM_OUTLINE]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ITEM::ITEM_OUTLINE]->Set_Texture();
	Update_Color(m_pTextureCom[(_uint)UI_ITEM::ITEM_OUTLINE]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ITEM::ITEM_INNER]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ITEM::ITEM_INNER]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ITEM::ITEM_RGBBOX]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ITEM::ITEM_RGBBOX]->Set_Texture();
	Update_Color(m_pTextureCom[(_uint)UI_ITEM::ITEM_RGBBOX]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ITEM::ITEM_TEXTBOX]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ITEM::ITEM_TEXTBOX]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ITEM::ITEM_EXECUTION]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ITEM::ITEM_EXECUTION]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ITEM::ITEM_MOUSE]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_ITEM::ITEM_MOUSE]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	if (!m_bTick)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_ITEM::ITEM_ICON]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_ITEM::ITEM_ICON]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUIItem::Add_Component()
{
	CComponent* pComponent = NULL;

	{
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ITEM::ITEM_OUTLINE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIItem_Outline"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Outline", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ITEM::ITEM_INNER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIItem_Inner"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Inner", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ITEM::ITEM_RGBBOX] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIItem_RGBBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RGBBox", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ITEM::ITEM_TEXTBOX] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIItem_TextBox"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_TextBox", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ITEM::ITEM_EXECUTION] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIItem_Execution"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Execution", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ITEM::ITEM_MOUSE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIItem_Mouse2"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Mouse", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_ITEM::ITEM_ICON] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DropKnifeTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Icon", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_ITEM::ITEM_OUTLINE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Outline", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ITEM::ITEM_INNER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Intter", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ITEM::ITEM_RGBBOX] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RGBBox", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ITEM::ITEM_TEXTBOX] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_TextBox", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ITEM::ITEM_EXECUTION] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Execution", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ITEM::ITEM_MOUSE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Mouse", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_ITEM::ITEM_ICON] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Icon", pComponent });
	}

	return S_OK;
}

void CUIItem::Update_Color(CTexture* _pTexture)
{
	m_fColor += 80.f;

	_float fHue = fmod(Engine::Get_Elapsed() * m_fColor, 360.0f);
	_float fSaturation = 1.0f;
	_float fValue = 1.0f;

	_pTexture->Change_TextureColor(fHue, fSaturation, fValue);
}

void CUIItem::Reset()
{
	m_bTick = false;
	m_fColor = 0.f;
	m_fScaleRatio = 4.f;
	m_fTick = 0.f;

	m_pTransformCom[(_uint)UI_ITEM::ITEM_OUTLINE]->Set_Pos(0.f, -150.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_INNER]->Set_Pos(0.f, -150.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_RGBBOX]->Set_Pos(0.f, -230.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_TEXTBOX]->Set_Pos(0.f, -218.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_EXECUTION]->Set_Pos(0.f, -245.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_MOUSE]->Set_Pos(0.f, -218.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_ICON]->Set_Pos(0.f, -WINCY / 2.f - 100.f, 0.f);

	m_pTransformCom[(_uint)UI_ITEM::ITEM_OUTLINE]->Set_Scale(57.f, 57.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_INNER]->Set_Scale(55.f, 55.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_RGBBOX]->Set_Scale(80.f, 30.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_TEXTBOX]->Set_Scale(77.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_EXECUTION]->Set_Scale(25.f, 25.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_MOUSE]->Set_Scale(50.f, 50.f, 0.f);
	m_pTransformCom[(_uint)UI_ITEM::ITEM_ICON]->Set_Scale(70.f, 70.f, 0.f);

	m_bRender = false;
}

void CUIItem::Free()
{
	Engine::CUIUnit::Free();
}