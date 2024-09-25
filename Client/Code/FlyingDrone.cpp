#include "pch.h"
#include "../Header/FlyingDrone.h"
#include "Export_System.h"
#include "Export_Utility.h"

CFlyingDrone::CFlyingDrone(LPDIRECT3DDEVICE9 _pGraphicDev) : 
	CDrone(_pGraphicDev)
{
}

CFlyingDrone::~CFlyingDrone()
{
}

CFlyingDrone* CFlyingDrone::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CFlyingDrone* pMonster = new CFlyingDrone(_pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("FlyingDrone Create Failed");
		return nullptr;
	}

	return pMonster;
}

HRESULT CFlyingDrone::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(10.f, 0.f, 25.f);

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetShow(true);
	Set_Animation();

	return S_OK;
}

HRESULT CFlyingDrone::Add_Component()
{
	CComponent* pComponent = NULL;

	//pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"Proto_TriCol"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlyingDroneAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_HEADSHOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlyingDroneDamagedTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HeadShotTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlyingDroneIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_WALK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlyingDroneAttackTex")); //Drone coming with AttackMotion
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_WalkTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_DAMAGED] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlyingDroneDamagedTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_DamagedTexture", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Calculator", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator", pComponent });

	return S_OK;
}

void CFlyingDrone::State_Check()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CDrone::DRONE_ATTACK:
			m_pAnimatorCom->PlayAnimation(L"Attack", true);
			break;
		case CDrone::DRONE_IDLE:
			m_pAnimatorCom->PlayAnimation(L"Idle", true);
			break;
		case CDrone::DRONE_WALK:
			m_pAnimatorCom->PlayAnimation(L"Walk", false);
			break;
		case CDrone::DRONE_DAMAGED:
			m_pAnimatorCom->PlayAnimation(L"Damaged", false);
			break;
		case CDrone::DRONE_HEADSHOT:
			m_pAnimatorCom->PlayAnimation(L"HeadShot", false);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CFlyingDrone::Attack(const _float& _fTimeDelta)
{
	_vec3 vPos, vPlayerPos, vDir;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Body_Transform"));
	NULL_CHECK(pPlayerTransform, -1);

	pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);

	vDir = vPlayerPos - vPos;

	if (10.f < D3DXVec3Length(&vDir))
	{
		Changing_State(CDrone::DRONE_IDLE);
	}
	else
	{
		Changing_State(CDrone::DRONE_ATTACK);
	}
}

void CFlyingDrone::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Idle", m_pTextureCom[DRONE_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack", m_pTextureCom[DRONE_ATTACK], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Damaged", m_pTextureCom[DRONE_DAMAGED], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Walk", m_pTextureCom[DRONE_WALK], 13.f);
	m_pAnimatorCom->CreateAnimation(L"HeadShot", m_pTextureCom[DRONE_HEADSHOT], 13.f);

	m_pAnimatorCom->PlayAnimation(L"Attack", true);
}

void CFlyingDrone::Free()
{
	CDrone::Free();
}
