#include "pch.h"
#include "..\Header\UIBossCount.h"
#include "Export_Utility.h"

CUIBossCount::CUIBossCount(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_iIndex(0)
	, m_iCount(0)
	, m_fDist(0.f)
{
}

CUIBossCount::~CUIBossCount()
{
}

CUIBossCount* CUIBossCount::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _int _iIndex)
{
	CUIBossCount* pUIBossCount = new CUIBossCount(_pGraphicDev);

	if (FAILED(pUIBossCount->Ready_Unit(_iIndex)))
	{
		Safe_Release(pUIBossCount);
		MSG_BOX("UIBossCount Create Failed");
		return nullptr;
	}

	return pUIBossCount;
}

HRESULT CUIBossCount::Ready_Unit(_int _iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_iIndex = _iIndex;
	m_fDist = 100.f;

	m_pTransformCom->Set_Pos(0.f, -170.f, 0.f);

	m_pTransformCom->Set_Scale(100.f, 100.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIBossCount::Update_Unit(const _float& _fTimeDelta)
{
	_int iOffset = m_iCount / 2;

	if (m_iCount % 2 == 1)
		m_pTransformCom->Set_Pos(-m_fDist * (float)iOffset + m_fDist * (float)m_iIndex, -170.f, 0.f);
	else
		m_pTransformCom->Set_Pos(-m_fDist / 2.f - m_fDist * (float)(iOffset - 1) + m_fDist * (float)m_iIndex, -170.f, 0.f);

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIBossCount::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIBossCount::Render_Unit()
{
	if (m_iIndex + 1 <= m_iCount)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUIBossCount::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_Count"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Count", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Count", pComponent });

	return S_OK;
}

void CUIBossCount::Reset()
{
	m_iCount = 0;
	m_fDist = 100.f;

	m_pTransformCom->Set_Pos(0.f, -170.f, 0.f);

	m_pTransformCom->Set_Scale(100.f, 100.f, 0.f);
}

void CUIBossCount::Free()
{
	Engine::CUIUnit::Free();
}