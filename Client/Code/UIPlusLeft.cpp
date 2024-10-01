#include "pch.h"
#include "..\Header\UIPlusLeft.h"
#include "Export_Utility.h"

CUIPlusLeft::CUIPlusLeft(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_fSpeed(0.f)
	, m_fTextTime(0.f)
	, m_fElapsed(0.f)
	, m_fScaleSpeed(0.f)
	, m_iTextIndex(0)
	, m_iSecIndex(0)
	, m_iIndex(0)
{
	for (_uint i = 0; i < (_uint)UI_PLUSLEFT::PLUSLEFT_END; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}

	for (int i = 0; i < 2; ++i)
	{
		m_fScaleRatio[i] = 0.f;
		m_fAlphaRatio[i] = 0.f;
	}
}

CUIPlusLeft::~CUIPlusLeft()
{
}

CUIPlusLeft* CUIPlusLeft::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIPlusLeft* pUIPlusLeft = new CUIPlusLeft(_pGraphicDev);

	if (FAILED(pUIPlusLeft->Ready_Unit()))
	{
		Safe_Release(pUIPlusLeft);
		MSG_BOX("UIPlusLeft Create Failed");
		return nullptr;
	}

	return pUIPlusLeft;
}

HRESULT CUIPlusLeft::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fScaleRatio[0] = 3.f;
	m_fScaleRatio[1] = 1.f;
	m_fAlphaRatio[0] = 0.f;
	m_fAlphaRatio[1] = 0.f;

	m_fSpeed = 4.f;
	m_fScaleSpeed = 1.f;
	m_fTextTime = 0.5f;

	for (_uint i = 0; i < (_uint)UI_PLUSLEFT::PLUSLEFT_END; ++i)
		m_pTransformCom[i]->Set_Angle(0.f, 0.f, D3DXToRadian(5.f));

	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Set_Pos(-WINCX * 0.3f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Set_Pos(-WINCX * 0.3f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_TEXT]->Set_Pos(-WINCX * 0.45f, -18.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT]->Set_Pos(-WINCX * 0.38f, -8.5f, 0.f);

	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Set_Scale(45.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Set_Scale(50.f, 50.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_TEXT]->Set_Scale(200.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT]->Set_Scale(55.f, 55.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIPlusLeft::Update_Unit(const _float& _fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Get_Info(INFO::INFO_UP, &vPos);

	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Set_Scale(45.f * m_fScaleRatio[0], 15.f * m_fScaleRatio[0], 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Set_Scale(50.f * m_fScaleRatio[0], 50.f * m_fScaleRatio[0], 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT]->Set_Scale(55.f * m_fScaleRatio[1], 55.f * m_fScaleRatio[1], 0.f);

	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Move_Pos(&vPos, _fTimeDelta, m_fSpeed);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Move_Pos(&vPos, _fTimeDelta, m_fSpeed);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_TEXT]->Move_Pos(&vPos, _fTimeDelta, m_fSpeed);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT]->Move_Pos(&vPos, _fTimeDelta, m_fSpeed);

	m_fElapsed += _fTimeDelta;

	m_fSpeed -= _fTimeDelta * 3.5f;
	if (m_fSpeed <= 0.f) m_fSpeed = -.8f;

	m_fScaleRatio[0] -= _fTimeDelta * 5.f;
	if (m_fScaleRatio[0] <= 1.f) m_fScaleRatio[0] = 1.f;

	m_fAlphaRatio[0] += _fTimeDelta * 2.f;
	if (m_fAlphaRatio[0] >= 1.f) m_fAlphaRatio[0] = 1.f;

	if (m_fTextTime + 0.1f <= m_fElapsed)
	{
		m_fScaleRatio[1] += m_fScaleSpeed * _fTimeDelta * 9.f;
		if (m_fScaleRatio[1] >= 3.f) m_fScaleSpeed = -m_fScaleSpeed;
		else if (m_fScaleRatio[1] <= 1.f) m_fScaleRatio[1] = 1.f;

		m_fAlphaRatio[1] += _fTimeDelta * 2.f;
		if (m_fAlphaRatio[1] >= 1.f) m_fAlphaRatio[1] = 1.f;
	}

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIPlusLeft::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIPlusLeft::Render_Unit()
{
	DWORD tPreFactor;

	m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);

	if (m_fTextTime <= m_fElapsed)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_TEXT]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Change_Alpha(m_fAlphaRatio[1]);
		m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_TEXT]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Change_Alpha(m_fAlphaRatio[0]);
	m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Change_Alpha(m_fAlphaRatio[0]);
	m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Set_Texture(m_iSecIndex);
	m_pBufferCom->Render_Buffer();

	if (m_fTextTime <= m_fElapsed)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT]->Set_Texture(m_iTextIndex);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
}

HRESULT CUIPlusLeft::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Back_Sec", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPlus_White"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Plus", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_TEXT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Back_Text", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Execution"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Text_Execution", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Back_Sec", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Plus", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_TEXT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Back_Text", pComponent });

	pComponent = m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Text_Execution", pComponent });

	return S_OK;
}

void CUIPlusLeft::Set_Text(_int _iIndex)
{
	m_iTextIndex = _iIndex;

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

void CUIPlusLeft::Reset()
{
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Set_Pos(-WINCX * 0.3f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Set_Pos(-WINCX * 0.3f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_TEXT]->Set_Pos(-WINCX * 0.45f, -18.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT]->Set_Pos(-WINCX * 0.38f, -8.5f, 0.f);

	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_SEC]->Set_Scale(45.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_SEC]->Set_Scale(50.f, 50.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_BACK_TEXT]->Set_Scale(200.f, 15.f, 0.f);
	m_pTransformCom[(_uint)UI_PLUSLEFT::PLUSLEFT_TEXT]->Set_Scale(55.f, 55.f, 0.f);

	m_fScaleRatio[0] = 3.f;
	m_fScaleRatio[1] = 1.f;
	m_fAlphaRatio[0] = 0.f;
	m_fAlphaRatio[1] = 0.f;

	m_fSpeed = 4.f;
	m_fScaleSpeed = 1.f;
	m_fTextTime = 0.5f;
	m_fElapsed = 0.f;
}

void CUIPlusLeft::Free()
{
	Engine::CUIUnit::Free();
}