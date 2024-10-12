#include "pch.h"
#include "../Header/Boss_Shield.h"
#include "Export_System.h"
#include "Export_Utility.h"

CBoss_Shield::CBoss_Shield(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_fFrame(0.f)
	, m_fMaxFrame(1.f)
	, m_fFrameSpeed(13.f)
	, m_Shield_Hp(10.f)
	, m_vOffset({ 0.f,0.f,10.f })
{
	m_bIsRender = false;
}

CBoss_Shield::~CBoss_Shield()
{
}

CBoss_Shield* CBoss_Shield::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CBoss_Shield* pShield = new CBoss_Shield(_pGraphicDev);

	if (FAILED(pShield->Ready_GameObject()))
	{
		Safe_Release(pShield);
		MSG_BOX("Shield Create Failed");
		return nullptr;
	}

	return pShield;
}

HRESULT CBoss_Shield::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(2.f);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(false);
	m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	return S_OK;
}

_int CBoss_Shield::Update_GameObject(const _float& _fTimeDelta)
{
	if (m_bIsRender) {
		Engine::Add_Collider(m_pColliderCom);
		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		_vec3 vPos;
		CTransform* m_pBossRoBotTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Monster", L"Boss_Robot", L"Com_Transform"));
		m_pBossRoBotTransformCom->Get_Info(INFO::INFO_POS, &vPos);

		CTransform* m_pPlayerTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform"));
		_vec3 vLook;
		m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vLook);

		_vec3 BuildingPos = { 20.f,25.f,45.f };
		_vec3 vDir = BuildingPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		vPos = vPos + vDir * 2.f;

		m_pTransformCom->Set_Pos(vPos);


		_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
		m_pTransformCom->LookAtTarget2(&vLook);
		return iExit;
	}
	return 0;
}

void CBoss_Shield::LateUpdate_GameObject()
{
	if (m_bIsRender) {
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		CGameObject::Compute_ViewZ(&vPos);
		m_fViewZ -= 5.f;
		if (m_bIsRender)
			m_pColliderCom->SetActive(true);
		else
			m_pColliderCom->SetActive(false);
		Engine::CGameObject::LateUpdate_GameObject();
	}
}

void CBoss_Shield::Render_GameObject()
{
	if (m_bIsRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

	}
}

void CBoss_Shield::OnCollision(CCollider& _pOther)
{
}

void CBoss_Shield::OnCollisionEnter(CCollider& _pOther)
{
}

void CBoss_Shield::OnCollisionExit(CCollider& _pOther)
{
}

void CBoss_Shield::Spawn_Shield(const _vec3& _vStartPos)
{
	m_pTransformCom->Set_Pos(_vStartPos);
	m_pTransformCom->Set_Scale(4.0f, 4.0f, 4.0f);
	m_fFrame = 0.f;
	m_bIsRender = true;
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);
}

HRESULT CBoss_Shield::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Boss_ShieldTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Boss_ShieldTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);

	return S_OK;
}

void CBoss_Shield::Free()
{
	CGameObject::Free();
}