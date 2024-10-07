#include "pch.h"
#include "../Header/ShotGun.h"
#include "Export_System.h"
#include "Export_Utility.h"

CShotGun::CShotGun(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CHumanoid(_pGraphicDev)
{
	m_fFireDelayTime = 4.f;
	m_fAttackTimer = 4.5f;
}

CShotGun::CShotGun(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
	: CHumanoid(_pGraphicDev)
{
	m_fFireDelayTime = 4.f;
	m_fAttackTimer = 4.5f;
	m_vStartPos = _vecPos;
}

CShotGun::~CShotGun()
{
}

CShotGun* CShotGun::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CShotGun* pMonster = new CShotGun(_pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("ShotGun Create Failed");
		return nullptr;
	}

	return pMonster;
}

CShotGun* CShotGun::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
{
	CShotGun* pMonster = new CShotGun(_pGraphicDev, _vecPos);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("ShotGun Create Failed");
		return nullptr;
	}

	return pMonster;
}

HRESULT CShotGun::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_vStartPos.y += 1.f;
	//m_pTransformCom->Set_Pos(9.f, 1.f, 21.f);
	m_pTransformCom->Set_Pos(m_vStartPos);
	_vec3 vDir = { 0.5f, 0.5f, 0.5f };

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetLookDir(vDir);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	Set_Animation();
	m_pHitBufferCom->SetvOffSet({ 0.f,0.f,0.f });
	m_pHeadHit->SetvOffSet({ 0.5f,0.5f,0.f });
	m_pCriticalHit->SetvOffSet({ -0.5f,0.5f,0.f });
	return S_OK;
}

_bool CShotGun::Get_Execution(_vec3 _vLook, const _bool& _bIsDo)
{
	if (!m_bIsExecution && _bIsDo)
	{
		m_bIsExecution = !m_bIsExecution;
		m_vPlayerLook = _vLook;
		Changing_State(CHumanoid::HUMANOID_EXECUTION);
		m_pAnimatorCom->PlayAnimation(L"Execution", false);
		m_fViewZ = 11.f;
		m_pTransformCom->Set_Scale({ 375.f, 375.f, 0.5f });
	}

	return true;
}

HRESULT CShotGun::Add_Component()
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

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_ATTACK_DELAY] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunAttackDelayTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackDelayTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_HEADSHOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunHeadShotTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HeadShotTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_BULLSHOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunBullShotTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_BullShotTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_PUSH_ONE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunPushOneTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_PushOneTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_PUSH_TWO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunPushTwoTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_PushTwoTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_SHOT_ONE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunShotOneTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShotOneTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_SHOT_TWO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunShotTwoTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShotTwoTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_KATANA] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunKatanaDownTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_KatanaTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_EXECUTION] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ShotGunExecutionTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ExecutionTexture", pComponent });

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

void CShotGun::State_Check()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CHumanoid::HUMANOID_IDLE:
			m_pAnimatorCom->PlayAnimation(L"Idle", true);
			break;
		case CHumanoid::HUMANOID_ATTACK_DELAY:
			m_pAnimatorCom->PlayAnimation(L"Attack_Delay", true);
			break;
		case CHumanoid::HUMANOID_ATTACK:
			m_pAnimatorCom->PlayAnimation(L"Attack", false);
			m_bIsFire = false;
			m_fAttackTimer = 0.f;
			break;
		case CHumanoid::HUMANOID_HEADSHOT:
			m_pAnimatorCom->PlayAnimation(L"HeadShot", false);
			break;
		case CHumanoid::HUMANOID_PUSH_ONE:
			m_pAnimatorCom->PlayAnimation(L"Push_One", false);
			break;
		case CHumanoid::HUMANOID_PUSH_TWO:
			m_pAnimatorCom->PlayAnimation(L"Push_Two", false);
			break;
		case CHumanoid::HUMANOID_BULLSHOT:
			m_pAnimatorCom->PlayAnimation(L"BullShot", false);
			break;
		case CHumanoid::HUMANOID_SHOT_ONE:
			m_pAnimatorCom->PlayAnimation(L"Shot_One", false);
			break;
		case CHumanoid::HUMANOID_SHOT_TWO:
			m_pAnimatorCom->PlayAnimation(L"Shot_Two", false);
			break;
		case CHumanoid::HUMANOID_KATANA:
			m_pAnimatorCom->PlayAnimation(L"Katana", false);
			break;
		case CHumanoid::HUMANOID_EXECUTION:
			m_pAnimatorCom->PlayAnimation(L"Execution", false);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CShotGun::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Idle", m_pTextureCom[HUMANOID_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack", m_pTextureCom[HUMANOID_ATTACK], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Delay", m_pTextureCom[HUMANOID_ATTACK_DELAY], 8.f);
	m_pAnimatorCom->CreateAnimation(L"HeadShot", m_pTextureCom[HUMANOID_HEADSHOT], 13.f);
	m_pAnimatorCom->CreateAnimation(L"BullShot", m_pTextureCom[HUMANOID_BULLSHOT], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Push_One", m_pTextureCom[HUMANOID_PUSH_ONE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Push_Two", m_pTextureCom[HUMANOID_PUSH_TWO], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Shot_One", m_pTextureCom[HUMANOID_SHOT_ONE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Shot_Two", m_pTextureCom[HUMANOID_SHOT_TWO], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Katana", m_pTextureCom[HUMANOID_KATANA], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Execution", m_pTextureCom[HUMANOID_EXECUTION], 15.f);

	m_pAnimatorCom->PlayAnimation(L"Idle", true);
}

void CShotGun::Attack(const _float& _fTimeDelta)
{
	if (m_bIsExecution && m_pAnimatorCom->GetCurrAnim()->GetFinish())
	{
		m_bIsExecution = false;
		Changing_State(CHumanoid::HUMANOID_IDLE);
		m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });
		AddForce(30.f, m_vPlayerLook, 15.f);
		CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectHeal", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectGreenFlash", L"Com_Effect");
		if (pComponent)
			static_cast<CEffect*>(pComponent)->Operate_Effect();

		m_fAttackTimer = 0.f;

		return;
	}
	else if (m_bIsExecution)
		return;

	_vec3 vPos, vPlayerPos, vDir, vUp, vRight;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
	vPos.y += 0.2f;

	if (nullptr == m_pPlayerTransformCom)
	{
		m_pPlayerTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform"));
		NULL_CHECK(m_pPlayerTransformCom, -1);
	}

	m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPos);

	vDir = vPlayerPos - vPos;
	D3DXVec3Cross(&vRight, &vUp, &vDir);
	D3DXVec3Normalize(&vRight, &vRight);
	vPos += (vRight * 0.2f);
	vDir = vPlayerPos - vPos;

	if (15.f < D3DXVec3Length(&vDir))
	{
		Changing_State(CHumanoid::HUMANOID_IDLE);
	}
	else //if (CHumanoid::HUMANOID_IDLE == m_eCurState || CHumanoid::HUMANOID_ATTACK_DELAY == m_eCurState)
	{
		if (m_fAttackTimer > m_fFireDelayTime || m_eCurState == CHumanoid::HUMANOID_IDLE)
		{
			if (!m_bIsExecution)
			{
				Changing_State(CHumanoid::HUMANOID_ATTACK);
				D3DXVec3Normalize(&vDir, &vDir);
				Engine::Fire_Bullet(m_pGraphicDev, vPos, vDir, 5, CBulletManager::BULLET_SHOTGUN);
				m_bIsFire = true;
			}
		}
		else if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
		{
			Changing_State(CHumanoid::HUMANOID_ATTACK_DELAY);
		}
	}

	if (m_eCurState == CHumanoid::HUMANOID_ATTACK_DELAY)
	{
		m_fAttackTimer += _fTimeDelta;
	}
}

void CShotGun::Free()
{
	CHumanoid::Free();
}
