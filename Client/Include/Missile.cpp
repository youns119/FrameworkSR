#include "pch.h"
#include "Missile.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "../Header/EffectPool.h"

CMissile::CMissile(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CBullet(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_fTimer(0.f)
	, m_fFrame(0.f)
	, m_fMaxFrame(4.f)
	, m_fLinear(0.f)
{
}

CMissile::~CMissile()
{
}

HRESULT CMissile::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(0.05f);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(false);
	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

_int CMissile::Update_GameObject(const _float& _fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_Collider(m_pColliderCom);
	_int iExit = Engine::CBullet::Update_GameObject(_fTimeDelta);

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


	if (m_bIsRender) {
		m_fLinear += 0.005f;
		if (m_fLinear >= 1.f) {
			m_fLinear = 0.f;
			m_bIsRender = false;
		}
		m_pTransformCom->Set_Pos(Bezier(m_vStart, m_vCurve, m_vEnd, m_fLinear));
	}
	CGameObject::Compute_ViewZ(&m_vDir);
	return iExit;
}

void CMissile::LateUpdate_GameObject()
{
	if (m_bIsRender)
		m_pColliderCom->SetActive(true);
	else
		m_pColliderCom->SetActive(false);
	Engine::CBullet::LateUpdate_GameObject();
}

void CMissile::Render_GameObject()
{
	if (m_bIsRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		m_pTextureCom->Set_Texture((_uint)m_fFrame);


		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void CMissile::Fire_Missile(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage, const _vec3& vCurvePos)
{
	m_bIsRender = true;
	m_pColliderCom->SetActive(true);
	m_vStart = _vStartPos;
	m_vEnd = _vEndPos;
	m_vCurve = vCurvePos;
	m_fAttackDamage = _fAttackDamage;
	m_pTransformCom->Set_Pos(_vStartPos);

	CGameObject* pGameObject = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_SmokeTrail", L"Com_Transform")->GetOwner();
	static_cast<CEffectPool*>(pGameObject)->Set_CallerObject(this);
	static_cast<CEffectPool*>(pGameObject)->Operate();
}

_vec3 CMissile::Bezier(_vec3 _vStartPos, _vec3 vCurvePos, _vec3 vEndPos, _float fTime)
{
	return (1 - fTime) * (1 - fTime) * _vStartPos +
		2 * (1 - fTime) * fTime * vCurvePos +
		fTime * fTime * vEndPos;
}

CMissile* CMissile::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CMissile* pMissile = new CMissile(_pGraphicDev);

	if (FAILED(pMissile->Ready_GameObject()))
	{
		Safe_Release(pMissile);
		MSG_BOX("Ammo Create Failed");
		return nullptr;
	}

	return pMissile;
}

HRESULT CMissile::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Missile"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AmmoTexture", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);

	return S_OK;
}

void CMissile::Free()
{
	CBullet::Free();
}
