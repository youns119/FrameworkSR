#include "pch.h"
#include "..\Header\UIIndicator.h"
#include "Export_Utility.h"
#include "..\Header\Player.h"

CUIIndicator::CUIIndicator(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
	m_eUIType = UITYPE::UI_INDICATOR;
}

CUIIndicator::~CUIIndicator()
{
}

CUIIndicator* CUIIndicator::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIIndicator* pUIIndicator = new CUIIndicator(_pGraphicDev);

	if (FAILED(pUIIndicator->Ready_UI()))
	{
		Safe_Release(pUIIndicator);
		MSG_BOX("UIIndicator create Failed");
		return nullptr;
	}

	return pUIIndicator;
}

HRESULT CUIIndicator::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(20.f, 20.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIIndicator::Update_UI(const _float& _fTimeDelta)
{
	if (!m_bRender)
		return 0;

	CComponent* pComponent = static_cast<CPlayer*>(m_pGameObject)->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider");
	_vec3 vColliderPos = static_cast<CCollider*>(pComponent)->GetFinalPos();
	vColliderPos = vColliderPos + _vec3(0.f, 2.f, 0.f);

	_vec3 vPos;
	_matrix matWorld, matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DVIEWPORT9 tViewport;
	m_pGraphicDev->GetViewport(&tViewport);

	D3DXVec3Project(&vPos, &vColliderPos, &tViewport, &matProj, &matView, &matWorld);

	m_pTransformCom->Set_Pos(vPos);

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIIndicator::LateUpdate_UI()
{
	if (!m_bRender)
		return;

	Engine::CUI::LateUpdate_UI();
}

void CUIIndicator::Render_UI()
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
	Engine::CUI::Free();
}