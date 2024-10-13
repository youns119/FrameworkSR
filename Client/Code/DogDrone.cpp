#include "pch.h"
#include "../Header/DogDrone.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "../Header/EffectPool.h"
#include "../Header/EffectMuzzleFlash.h"

CDogDrone::CDogDrone(LPDIRECT3DDEVICE9 _pGraphicDev) : 
	CDrone(_pGraphicDev)
{
	m_fHP = 8.f;
}

CDogDrone::CDogDrone(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
	: CDrone(_pGraphicDev)
{
	m_fHP = 8.f;
	m_vStartPos = _vecPos;
}

CDogDrone::~CDogDrone()
{
}

CDogDrone* CDogDrone::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CDogDrone* pMonster = new CDogDrone(_pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("DogDrone Create Failed");
		return nullptr;
	}

	return pMonster;
}

CDogDrone* CDogDrone::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
{
	CDogDrone* pMonster = new CDogDrone(_pGraphicDev, _vecPos);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("DogDrone Create Failed");
		return nullptr;
	}

	return pMonster;
}

HRESULT CDogDrone::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Set_Pos(20.f, 0.f, 20.f);
	m_vStartPos.y += 1.f;
	m_pTransformCom->Set_Pos(m_vStartPos);
	_vec3 vDir = { 0.5f, 0.5f, 0.5f };

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(0.75f);
	m_pColliderCom->SetLookDir(vDir);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	m_pHitBufferCom->SetvOffSet({ 0.f,0.f,0.f });
	m_pHeadHit->SetvOffSet({ 0.5f,0.5f,0.f });
	m_pCriticalHit->SetvOffSet({ -0.5f,0.5f,0.f });

	Set_Animation();


	return S_OK;
}

HRESULT CDogDrone::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pHitBufferCom = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"Proto_HitBufferCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HitBufferCom", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pHeadHit = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"Proto_HitBufferCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HeadHit", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pCriticalHit = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"Proto_HitBufferCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_CriticalHit", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DogDroneAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_HEADSHOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DogDroneHeadShotTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HeadShotTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DogDroneIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_WALK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DogDroneWalkTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_WalkTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_DAMAGED] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DogDroneDamagedTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_DamagedTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_KATANA] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DogDroneKatanaRightTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_KatanaRightTexture", pComponent });

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

void CDogDrone::State_Check()
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
			m_pAnimatorCom->PlayAnimation(L"Walk", true);
			Engine::Play_Sound(L"Drone_Walk.wav", CHANNELID::SOUND_ENEMY_GUN, 0.7f);
			break;
		case CDrone::DRONE_DAMAGED:
			m_pAnimatorCom->PlayAnimation(L"Damaged", false);
			break;
		case CDrone::DRONE_HEADSHOT:
			m_pAnimatorCom->PlayAnimation(L"HeadShot", false);
			break;
		case CDrone::DRONE_KATANA:
			m_pAnimatorCom->PlayAnimation(L"Katana", false);
			break;
		}

		m_ePreState = m_eCurState;

		
		
	}

	if (!m_bIsExplosion)
	{
		if (CDrone::DRONE_DAMAGED == m_eCurState || CDrone::DRONE_KATANA == m_eCurState) //죽었을때(HeadShot 제외)
		{
			if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
			{
				CComponent* pComponent(nullptr);
				CGameObject* pGameObject(nullptr);

				_vec3 vPos, vLook;
				m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
				m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);

				pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_Explosion", L"Com_Transform");
				static_cast<CTransform*>(pComponent)->Set_Pos(vPos + vLook * 1.f);
				pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
				static_cast<CEffectPool*>(pGameObject)->Set_CallerObject(this);
				static_cast<CEffectPool*>(pGameObject)->Operate();
				m_bIsRender = false;
				m_bIsExplosion = true;
			}
		}
	}
}

void CDogDrone::Attack(const _float& _fTimeDelta)
{
	_vec3 vPos, vPlayerPos, vDir, vKnockBack;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	if (nullptr == m_pPlayerTransformCom)
	{
		m_pPlayerTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform"));
		NULL_CHECK(m_pPlayerTransformCom, -1);
	}

	m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPos);

	vDir = vPlayerPos - vPos;
	_float fDistance(0.f);
	fDistance = D3DXVec3Length(&vDir);
	if (10.f < fDistance)
	{
		Changing_State(CDrone::DRONE_IDLE);
	}
	else
	{
		Changing_State(CDrone::DRONE_WALK);
		if (1.2f > fDistance)
		{
			D3DXVec3Normalize(&vKnockBack, &vDir);
			vKnockBackForce = (vKnockBack * -20.f);
		}
		else
			AttackMoving(_fTimeDelta, vDir);
	}
}

void CDogDrone::AttackMoving(const _float& _fTimeDelta, const _vec3& _vDir)
{
	_vec3 vMove, vPos, vFinal;
	D3DXVec3Normalize(&vMove, &_vDir);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	vFinal = vPos + (vMove * _fTimeDelta * 1.5f);
	m_pTransformCom->Set_Pos(vFinal);
}

void CDogDrone::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Idle", m_pTextureCom[DRONE_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack", m_pTextureCom[DRONE_ATTACK], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Damaged", m_pTextureCom[DRONE_DAMAGED], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Walk", m_pTextureCom[DRONE_WALK], 13.f);
	m_pAnimatorCom->CreateAnimation(L"HeadShot", m_pTextureCom[DRONE_HEADSHOT], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Katana", m_pTextureCom[DRONE_KATANA], 13.f);

	m_pAnimatorCom->PlayAnimation(L"Attack", true);
}

void CDogDrone::Free()
{
	CDrone::Free();
}
