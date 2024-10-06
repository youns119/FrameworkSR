#include "pch.h"
#include "../Header/Shield.h"
#include "Export_System.h"
#include "Export_Utility.h"

CShield::CShield(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_fFrame(0.f)
	, m_fMaxFrame(10.f)
	, m_fFrameSpeed(13.f)
{
	m_bIsRender = false;
}

CShield::~CShield()
{
}

CShield* CShield::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CShield* pShield = new CShield(_pGraphicDev);

	if (FAILED(pShield->Ready_GameObject()))
	{
		Safe_Release(pShield);
		MSG_BOX("Shield Create Failed");
		return nullptr;
	}

	return pShield;
}

HRESULT CShield::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CShield::Update_GameObject(const _float& _fTimeDelta)
{
	m_fFrame += m_fFrameSpeed * _fTimeDelta;

	if (m_fMaxFrame < m_fFrame) // Jonghan Change
	{
		m_fFrame = 0.f;
		m_bIsRender = false;
	}

	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);

	_matrix		matWorld, matView, matBill, matResult;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	matResult = matBill * matWorld;

	m_pTransformCom->Set_WorldMatrix(&(matResult));

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	return iExit;
}

void CShield::LateUpdate_GameObject()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject();

}

void CShield::Render_GameObject()
{
	if (m_bIsRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		m_pTextureCom->Set_Texture((_uint)m_fFrame); //Jonghan Change

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void CShield::Spawn_Shield(const _vec3& _vStartPos)
{
	_vec3 vPos = _vStartPos;

	m_pTransformCom->Set_Pos(_vStartPos.x, _vStartPos.y, _vStartPos.z);
	m_fFrame = 0.f;
	m_bIsRender = true;
}

HRESULT CShield::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShieldTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShieldTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CShield::Free()
{
	CGameObject::Free();
}
