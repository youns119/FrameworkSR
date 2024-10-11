#include "pch.h"
#include "..\Header\UIDownBase.h"
#include "Export_Utility.h"

CUIDownBase::CUIDownBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_bTick(false)
	, m_fTimeDelay(0.f)
	, m_fTimer(0.f)
	, m_fTick(0.f)
	, m_fScaleRatio(0.f)
	, m_fAlpha(0.f)
{
	for (_uint i = 0; i < (_uint)UI_DOWN::DOWN_END; i++)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUIDownBase::~CUIDownBase()
{
}

CUIDownBase* CUIDownBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIDownBase* pUIDownBase = new CUIDownBase(_pGraphicDev);

	if (FAILED(pUIDownBase->Ready_Unit()))
	{
		Safe_Release(pUIDownBase);
		MSG_BOX("UIDownBase create Failed");
		return nullptr;
	}

	return pUIDownBase;
}

HRESULT CUIDownBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fScaleRatio = 3.f;

	m_pTransformCom[(_uint)UI_DOWN::DOWN_BLACK]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_DOWN::DOWN_SEC]->Set_Pos(0.f, 0.f, 0.f);

	m_pTransformCom[(_uint)UI_DOWN::DOWN_BLACK]->Set_Scale(55.f, 25.f, 0.f);
	m_pTransformCom[(_uint)UI_DOWN::DOWN_SEC]->Set_Scale(80.f, 80.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIDownBase::Update_Unit(const _float& _fTimeDelta)
{
	m_fTimer += _fTimeDelta;
	if (m_fTimer < m_fTimeDelay) return 0;

	if (m_fTimer >= 1.3f)
	{
		m_fTick += _fTimeDelta;

		if (m_fTick >= 0.1f)
		{
			m_fTick = 0.f;
			m_bTick = !m_bTick;
		}
	}

	if (m_fScaleRatio > 1.f)
	{
		m_fScaleRatio -= _fTimeDelta * 10.f;
		if (m_fScaleRatio <= 1.f) m_fScaleRatio = 1.f;

		m_pTransformCom[(_uint)UI_DOWN::DOWN_BLACK]->Set_Scale(55.f * m_fScaleRatio, 25.f * m_fScaleRatio, 0.f);
		m_pTransformCom[(_uint)UI_DOWN::DOWN_SEC]->Set_Scale(80.f * m_fScaleRatio, 80.f * m_fScaleRatio, 0.f);
	}

	_vec3 vDir = { 0.f, 1.f, 0.f };
	m_pTransformCom[(_uint)UI_DOWN::DOWN_BLACK]->Move_Pos(&vDir, _fTimeDelta, 120.f);
	m_pTransformCom[(_uint)UI_DOWN::DOWN_SEC]->Move_Pos(&vDir, _fTimeDelta, 120.f);

	if (m_fAlpha < 1.f)
	{
		m_fAlpha += _fTimeDelta;
		if (m_fAlpha >= 1.f) m_fAlpha = 1.f;
	}

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIDownBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIDownBase::Render_Unit()
{
	if (m_fTimeDelay > m_fTimer || m_fTick)
		return;

	DWORD tPreFactor;
	m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_DOWN::DOWN_BLACK]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_DOWN::DOWN_BLACK]->Set_Texture();
	m_pTextureCom[(_uint)UI_DOWN::DOWN_BLACK]->Change_Alpha(m_fAlpha);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_DOWN::DOWN_SEC]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_DOWN::DOWN_SEC]->Set_Texture();
	m_pTextureCom[(_uint)UI_DOWN::DOWN_SEC]->Change_Alpha(m_fAlpha);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
}

HRESULT CUIDownBase::Add_Component()
{
	CComponent* pComponent = NULL;

	{
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_DOWN::DOWN_BLACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIDown_Black"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Black", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_DOWN::DOWN_SEC] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIDown_Second"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Second", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_DOWN::DOWN_BLACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Black", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_DOWN::DOWN_SEC] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Second", pComponent });
	}

	return S_OK;
}

void CUIDownBase::Init(_int _iIndex)
{
	_vec3 vPos;

	vPos = { -250.f + rand() % 100, -300.f + rand() % 100, 0.f };

	m_pTransformCom[(_uint)UI_DOWN::DOWN_BLACK]->Set_Pos(vPos);
	m_pTransformCom[(_uint)UI_DOWN::DOWN_SEC]->Set_Pos(vPos);

	m_fTimeDelay = 0.4f * (_float)_iIndex;
}

void CUIDownBase::Reset()
{
	m_bTick = false;
	m_fTimeDelay = 0.f;
	m_fTimer = 0.f;
	m_fTick = 0.f;
	m_fScaleRatio = 3.f;
	m_fAlpha = 0.f;

	m_pTransformCom[(_uint)UI_DOWN::DOWN_BLACK]->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom[(_uint)UI_DOWN::DOWN_SEC]->Set_Pos(0.f, 0.f, 0.f);

	m_pTransformCom[(_uint)UI_DOWN::DOWN_BLACK]->Set_Scale(55.f, 25.f, 0.f);
	m_pTransformCom[(_uint)UI_DOWN::DOWN_SEC]->Set_Scale(80.f, 80.f, 0.f);
}

void CUIDownBase::Free()
{
	Engine::CUIUnit::Free();
}