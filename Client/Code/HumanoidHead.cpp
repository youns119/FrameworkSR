#include "pch.h"
#include "../Header/HumanoidHead.h"
#include "Export_System.h"
#include "Export_Utility.h"

CHumanoidHead::CHumanoidHead(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CBullet(_pGraphicDev)
	, m_fFrame(0.f)
	, m_fLinear(0.f)
	, m_fMaxFrame(20.f)
	, m_fTimer(0.f)
	, m_pBufferCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_vCurve(0.f, 0.f, 0.f)
	, m_vEnd(0.f, 0.f, 0.f)
	, m_vStart(0.f, 0.f, 0.f)
{
}

CHumanoidHead::~CHumanoidHead()
{
}

HRESULT CHumanoidHead::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(0.05f);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(false);
	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

_int CHumanoidHead::Update_GameObject(const _float& _fTimeDelta)
{
	if (m_bIsRender) {
		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		Engine::Add_Collider(m_pColliderCom);
		_int iExit = Engine::CBullet::Update_GameObject(_fTimeDelta);
		m_fFrame += 13.f * _fTimeDelta;

		if (m_fMaxFrame < m_fFrame) // Jonghan Change
			m_fFrame = 0.f;

		if (8.f < m_fTimer)
		{
			m_bIsRender = false;
			m_pColliderCom->SetShow(false);
			m_pColliderCom->SetActive(false);
			m_fTimer = 0.f;
		}

		m_fLinear += 0.2f * _fTimeDelta;
		if (m_fLinear >= 1.f) {
			m_fLinear = 0.f;
			m_bIsRender = false;
			m_pColliderCom->SetActive(false);
		}

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
		_vec3 vPos, vNext;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

		m_pTransformCom->Set_WorldMatrix(&(matResult));

		m_pTransformCom->Set_Pos(Bezier(m_vStart, m_vCurve, m_vEnd, m_fLinear));
		vNext = Bezier(m_vStart, m_vCurve, m_vEnd, m_fLinear + 0.1f);
		//m_pTransformCom->LookAtTarget(&vNext);

		CGameObject::Compute_ViewZ(&m_vDir);
		return iExit;
	}
}

void CHumanoidHead::LateUpdate_GameObject()
{
	if (m_bIsRender)
		m_pColliderCom->SetActive(true);
	else
		m_pColliderCom->SetActive(false);
	Engine::CBullet::LateUpdate_GameObject();
}

void CHumanoidHead::Render_GameObject()
{
	if (m_bIsRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		m_pTextureCom->Set_Texture((_uint)m_fFrame);


		m_pBufferCom->Render_Buffer();

		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

void CHumanoidHead::Fire_Head(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage, const _vec3& vCurvePos)
{
	m_bIsRender = true;
	m_pColliderCom->SetActive(true);
	m_vStart = _vStartPos;
	m_vEnd = _vEndPos;
	m_vCurve = vCurvePos;
	m_fAttackDamage = _fAttackDamage;
	m_pTransformCom->Set_Pos(_vStartPos);

	//CGameObject* pGameObject = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_SmokeTrail", L"Com_Transform")->GetOwner();
	//static_cast<CEffectPool*>(pGameObject)->Set_CallerObject(this);
	//static_cast<CEffectPool*>(pGameObject)->Operate();
}

_vec3 CHumanoidHead::Bezier(_vec3 _vStartPos, _vec3 vCurvePos, _vec3 vEndPos, _float fTime)
{
	return (1 - fTime) * (1 - fTime) * _vStartPos +
		2 * (1 - fTime) * fTime * vCurvePos +
		fTime * fTime * vEndPos;
}

CHumanoidHead* CHumanoidHead::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CHumanoidHead* pHumanoidHead = new CHumanoidHead(_pGraphicDev);

	if (FAILED(pHumanoidHead->Ready_GameObject()))
	{
		Safe_Release(pHumanoidHead);
		MSG_BOX("HumanoidHead Create Failed");
		return nullptr;
	}

	return pHumanoidHead;
}

HRESULT CHumanoidHead::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossHumanoidHead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HeadTexture", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);
	return S_OK;
}

void CHumanoidHead::Free()
{
	CBullet::Free();
}
