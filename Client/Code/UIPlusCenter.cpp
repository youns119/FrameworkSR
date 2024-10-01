#include "pch.h"
#include "..\Header\UIPlusCenter.h"
#include "Export_Utility.h"
#include "Export_System.h"

CUIPlusCenter::CUIPlusCenter(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_vPos{}
	, m_fScaleRatio(0.f)
	, m_fAlphaRatio(0.f)
	, m_iSecIndex(0)
{
	for (_uint i = 0; i < (_uint)UI_PLUSCENTER::PLUSCENTER_END; ++i)
	{
		m_pTransformCom[i] = nullptr;
		m_pTextureCom[i] = nullptr;
	}
}

CUIPlusCenter::~CUIPlusCenter()
{
}

CUIPlusCenter* CUIPlusCenter::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIPlusCenter* pUIPlusCenter = new CUIPlusCenter(_pGraphicDev);

	if (FAILED(pUIPlusCenter->Ready_Unit()))
	{
		Safe_Release(pUIPlusCenter);
		MSG_BOX("UIPlusCenter Create Failed");
		return nullptr;
	}

	return pUIPlusCenter;
}

HRESULT CUIPlusCenter::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fScaleRatio = 3.f;
	m_fAlphaRatio = 0.f;

	for (_uint i = 0; i < (_uint)UI_PLUSCENTER::PLUSCENTER_END; ++i)
		m_pTransformCom[i]->Set_Pos(m_vPos);

	m_pTransformCom[(_uint)UI_PLUSCENTER::PLUSCENTER_BACK]->Set_Scale(70.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSCENTER::PLUSCENTER_SEC]->Set_Scale(80.f * m_fScaleRatio, 80.f * m_fScaleRatio, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIPlusCenter::Update_Unit(const _float& _fTimeDelta)
{
	m_vPos.y = m_vPos.y + _fTimeDelta * 7.f;

	D3DVIEWPORT9 tViewport;
	_matrix matWorld, matView, matProj;

	m_pGraphicDev->GetViewport(&tViewport);
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vScreenPos{};
	D3DXVec3Project(&vScreenPos, &m_vPos, &tViewport, &matProj, &matView, &matWorld);

	if (vScreenPos.z <= 0.f || vScreenPos.z >= 1.f)
		vScreenPos.y = -vScreenPos.y;

	vScreenPos.x -= WINCX * 0.5f;
	vScreenPos.y = -vScreenPos.y + WINCY * 0.5f;
	vScreenPos.z = 0.f;

	if (vScreenPos.x <= -WINCX * 0.4f) vScreenPos.x = -WINCX * 0.4f;
	else if (vScreenPos.x >= WINCX * 0.4f) vScreenPos.x = WINCX * 0.4f;
	if (vScreenPos.y <= -WINCY * 0.35f) vScreenPos.y = WINCY * 0.35f;
	else if (vScreenPos.y >= WINCY * 0.35f) vScreenPos.y = WINCY * 0.35f;

	for (_uint i = 0; i < (_uint)UI_PLUSCENTER::PLUSCENTER_END; ++i)
		m_pTransformCom[i]->Set_Pos(vScreenPos);

	m_pTransformCom[(_uint)UI_PLUSCENTER::PLUSCENTER_SEC]->Set_Scale(80.f * m_fScaleRatio, 80.f * m_fScaleRatio, 0.f);

	m_fScaleRatio -= _fTimeDelta * 5.f;
	if (m_fScaleRatio <= 1.f) m_fScaleRatio = 1.f;

	m_fAlphaRatio += _fTimeDelta * 2.f;
	if (m_fAlphaRatio >= 1.f) m_fAlphaRatio = 1.f;

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIPlusCenter::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIPlusCenter::Render_Unit()
{
	DWORD tPreFactor;

	m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PLUSCENTER::PLUSCENTER_BACK]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_PLUSCENTER::PLUSCENTER_BACK]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PLUSCENTER::PLUSCENTER_SEC]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_PLUSCENTER::PLUSCENTER_BACK]->Change_Alpha(m_fAlphaRatio);
	m_pTextureCom[(_uint)UI_PLUSCENTER::PLUSCENTER_SEC]->Set_Texture(m_iSecIndex);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
}

HRESULT CUIPlusCenter::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_PLUSCENTER::PLUSCENTER_BACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Back", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_PLUSCENTER::PLUSCENTER_SEC] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPlus_White"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Plus", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_PLUSCENTER::PLUSCENTER_BACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Back", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_PLUSCENTER::PLUSCENTER_SEC] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Plus", pComponent });

	return S_OK;
}

void CUIPlusCenter::Set_Sec(_int _iIndex)
{
	switch (_iIndex)
	{
	case 1:
	case 2:
	case 10:
	case 12:
		m_iSecIndex = 1;
		break;
	case 0:
	case 6:
	case 7:
	case 8:
		m_iSecIndex = 2;
		break;
	case 4:
	case 9:
	case 11:
		m_iSecIndex = 3;
		break;
	case 5:
		m_iSecIndex = 4;
		break;
	case 3:
		m_iSecIndex = 9;
		break;
	default:
		break;
	}
}

void CUIPlusCenter::Reset()
{
	m_vPos = { 0.f, 0.f, 0.f };
	m_fScaleRatio = 2.f;
	m_fAlphaRatio = 0.f;
}

void CUIPlusCenter::Free()
{
	Engine::CUIUnit::Free();
}

//void CUIPlusCenter::Update_Color()
//{
//	_float fHue = fmod(Engine::Get_Elapsed() * 70.f, 360.0f);
//	_float fSaturation = 1.0f;
//	_float fValue = 1.0f;
//
//	m_pTextureCom->Change_TextureColor(fHue, fSaturation, fValue);
//}