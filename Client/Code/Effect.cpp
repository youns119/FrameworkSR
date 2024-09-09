#include "pch.h"
#include "..\Header\Effect.h"
#include "Export_Utility.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CGameObject(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_fFrame(0.f)
{
}

CEffect::~CEffect()
{
}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEffect* pEffect = new CEffect(_pGraphicDev);

	if (FAILED(pEffect->Ready_GameObject()))
	{
		Safe_Release(pEffect);
		MSG_BOX("pEffect Create Failed");
		return nullptr;
	}

	return pEffect;
}

HRESULT CEffect::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(_float(rand() % 20), 0.f, _float(rand() % 20));

	return S_OK;
}

_int CEffect::Update_GameObject(const _float& _fTimeDelta)
{
	m_fFrame += 90.f * _fTimeDelta;

	if (90.f < m_fFrame)
		m_fFrame = 0.f;

	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);

	_matrix	matWorld, matView, matBill;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	matWorld = matBill * matWorld;
	m_pTransformCom->Set_WorldMatrix(&matWorld);

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	return iExit;
}

void CEffect::LateUpdate_GameObject()
{
	_vec3 vTemp;

	m_pTransformCom->Get_Info(Engine::INFO::INFO_POS, &vTemp);
	CGameObject::Compute_ViewZ(&vTemp);

	Engine::CGameObject::LateUpdate_GameObject();
}

void CEffect::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CEffect::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_EffectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CEffect::Free()
{
	Engine::CGameObject::Free();
}