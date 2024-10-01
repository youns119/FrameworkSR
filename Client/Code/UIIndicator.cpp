#include "pch.h"
#include "..\Header\UIIndicator.h"
#include "Export_Utility.h"

CUIIndicator::CUIIndicator(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_vPos{}
{
}

CUIIndicator::~CUIIndicator()
{
}

CUIIndicator* CUIIndicator::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIIndicator* pUIIndicator = new CUIIndicator(_pGraphicDev);

	if (FAILED(pUIIndicator->Ready_Unit()))
	{
		Safe_Release(pUIIndicator);
		MSG_BOX("UIIndicator create Failed");
		return nullptr;
	}

	return pUIIndicator;
}

HRESULT CUIIndicator::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(25.f, 25.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIIndicator::Update_Unit(const _float& _fTimeDelta)
{
	D3DVIEWPORT9 tViewport;
	_matrix matWorld, matView, matProj;

	m_pGraphicDev->GetViewport(&tViewport);
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vScreenPos{};
	D3DXVec3Project(&vScreenPos, &m_vPos, &tViewport, &matProj, &matView, &matWorld);

	if (vScreenPos.z < 0.f || vScreenPos.z > 1.f)
		return 0;

	vScreenPos.x -= WINCX * 0.5f;
	vScreenPos.y = -vScreenPos.y + WINCY * 0.5f;
	vScreenPos.z = 0.f;

	if (vScreenPos.x <= -WINCX * 0.5f) vScreenPos.x = -WINCX * 0.5f;
	else if (vScreenPos.x >= WINCX * 0.5f) vScreenPos.x = WINCX * 0.5f;
	if (vScreenPos.y <= -WINCY * 0.5f) vScreenPos.y = -WINCY * 0.5f;
	else if (vScreenPos.y >= WINCY * 0.5f) vScreenPos.y = WINCY * 0.5f;

	m_pTransformCom->Set_Pos(vScreenPos);

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIIndicator::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIIndicator::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIIndicator::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIIndicator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CUIIndicator::Free()
{
	Engine::CUIUnit::Free();
}