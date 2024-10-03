#include "pch.h"
#include "../Header/Boss_Robot.h"
#include "Export_System.h"
#include "Export_Utility.h"

CBoss_Robot::CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CMonster(_pGraphicDev)
	, m_eCurState(CBoss_Robot::BOSS_TALKING)
	, m_ePreState(CBoss_Robot::BOSS_TALKING)
	, m_eCurHealthState(CBoss_Robot::BOSSHEALTH_NORMAL)
	, m_fBoss_HP(100.f)
	, m_fShield_HP(0.f)
	, m_fDelayTime(4.8f)
	, m_fAttackTime(5.0f)
	, m_fAngle(180.f)
{
	for (_int i = 0; i < CBoss_Robot::BOSS_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CBoss_Robot::CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
	: CMonster(_pGraphicDev)
	, m_eCurState(CBoss_Robot::BOSS_TALKING)
	, m_ePreState(CBoss_Robot::BOSS_TALKING)
	, m_eCurHealthState(CBoss_Robot::BOSSHEALTH_NORMAL)
	, m_fBoss_HP(100.f)
	, m_fShield_HP(0.f)
{
	for (_int i = 0; i < CBoss_Robot::BOSS_END; ++i)
		m_pTextureCom[i] = nullptr;

	m_vStartPos = _vecPos;
}

CBoss_Robot::~CBoss_Robot()
{
}

CBoss_Robot* CBoss_Robot::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CBoss_Robot* pGameObject = new CBoss_Robot(_pGraphicDev);

	if (FAILED(pGameObject->Ready_GameObject()))
	{
		Safe_Release(pGameObject);
		MSG_BOX("Boss_Robot Create Failed");
		return nullptr;
	}

	return pGameObject;
}

CBoss_Robot* CBoss_Robot::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
{
	CBoss_Robot* pGameObject = new CBoss_Robot(_pGraphicDev, _vecPos);

	if (FAILED(pGameObject->Ready_GameObject()))
	{
		Safe_Release(pGameObject);
		MSG_BOX("Boss_Robot Create Failed");
		return nullptr;
	}

	return pGameObject;
}

HRESULT CBoss_Robot::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(10.f, 5.f, 25.f);

	_vec3 vDir = { 0.5f, 0.5f, 0.5f };

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetLookDir(vDir);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	Set_Animation();

	return S_OK;
}

void CBoss_Robot::LateUpdate_GameObject()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

	CGameObject::Compute_ViewZ(&vPos);

	//Change_State(); //this is Instance code for check Animation of Monster

	State_Check();

	Engine::CGameObject::LateUpdate_GameObject();
}

void CBoss_Robot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//Jonghan Monster Change Start

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pAnimatorCom->Render_Animator();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBoss_Robot::Damaged_By_Player(const DAMAGED_STATE& _eDamagedState, const _float& _fAttackDamage)
{
	if (Check_Phase())
		m_fShield_HP -= _fAttackDamage;
	else
		m_fBoss_HP -= _fAttackDamage;
}



HRESULT CBoss_Robot::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pHitBufferCom = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"Proto_HitBufferCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HitBufferCom", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_IDLE_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleNormalTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_IDLE_DAMAGED] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotDamagedIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleDamagedTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_IDLE_MOREDAMAGED] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotMoreDamagedIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleMoreDamagedTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_ATTACK_NORMAL_TWOHAND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotAttackTwoHandTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackNormalTwoHandTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_ATTACK_NORMAL_ONEHAND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotAttackOneHandTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackNormalOneHandTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_ATTACK_DAMAGED_TWOHAND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotDamagedAttackTwoHandTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackDamagedTwoHandTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_ATTACK_DAMAGED_ONEHAND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotDamagedAttackOneHandTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackDamagedOneHandTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_HIT_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotHitTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HitNormalTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_HIT_DAMAGED] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotDamagedHitTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HitDamagedTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_SHIELD_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotShieldTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShieldNormalTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_SHIELD_DAMAGED] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotDamagedShieldTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShieldDamagedTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Robot::BOSS_TALKING] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobotTalkingTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_TalkingTexture", pComponent });

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

void CBoss_Robot::State_Check()//This Function Calling in Monster.cpp -> LateUpdate
{
	//This function should Add Code about Change_Phase refer to Boss_HP || Shield_HP

	//Jonghan Test Code Start-> You should Delete it
	if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
	{
		switch (m_eCurState)
		{
		case CBoss_Robot::BOSS_IDLE_NORMAL:
			m_eCurState = CBoss_Robot::BOSS_IDLE_DAMAGED;
			break;
		case CBoss_Robot::BOSS_IDLE_DAMAGED:
			m_eCurState = CBoss_Robot::BOSS_IDLE_MOREDAMAGED;
			break;
		case CBoss_Robot::BOSS_IDLE_MOREDAMAGED:
			m_eCurState = CBoss_Robot::BOSS_ATTACK_NORMAL_TWOHAND;
			break;
		case CBoss_Robot::BOSS_ATTACK_NORMAL_TWOHAND:
			m_eCurState = CBoss_Robot::BOSS_ATTACK_NORMAL_ONEHAND;
			break;
		case CBoss_Robot::BOSS_ATTACK_NORMAL_ONEHAND:
			m_eCurState = CBoss_Robot::BOSS_ATTACK_DAMAGED_TWOHAND;
			break;
		case CBoss_Robot::BOSS_ATTACK_DAMAGED_TWOHAND:
			m_eCurState = CBoss_Robot::BOSS_ATTACK_DAMAGED_ONEHAND;
			break;
		case CBoss_Robot::BOSS_ATTACK_DAMAGED_ONEHAND:
			m_eCurState = CBoss_Robot::BOSS_HIT_NORMAL;
			break;
		case CBoss_Robot::BOSS_HIT_NORMAL:
			m_eCurState = CBoss_Robot::BOSS_HIT_DAMAGED;
			break;
		case CBoss_Robot::BOSS_HIT_DAMAGED:
			m_eCurState = CBoss_Robot::BOSS_SHIELD_NORMAL;
			break;
		case CBoss_Robot::BOSS_SHIELD_NORMAL:
			m_eCurState = CBoss_Robot::BOSS_SHIELD_DAMAGED;
			break;
		case CBoss_Robot::BOSS_SHIELD_DAMAGED:
			m_eCurState = CBoss_Robot::BOSS_TALKING;
			break;
		case CBoss_Robot::BOSS_TALKING:
			m_eCurState = CBoss_Robot::BOSS_IDLE_NORMAL;
			break;
		}

		if (m_eCurState != m_ePreState)
		{
			switch (m_eCurState)
			{
			case CBoss_Robot::BOSS_IDLE_NORMAL:
				m_pAnimatorCom->PlayAnimation(L"Idle_Normal", false);
				break;
			case CBoss_Robot::BOSS_IDLE_DAMAGED:
				m_pAnimatorCom->PlayAnimation(L"Idle_Damaged", false);
				break;
			case CBoss_Robot::BOSS_IDLE_MOREDAMAGED:
				m_pAnimatorCom->PlayAnimation(L"Idle_MoreDamaged", false);
				break;
			case CBoss_Robot::BOSS_ATTACK_NORMAL_TWOHAND:
				m_pAnimatorCom->PlayAnimation(L"Attack_Normal_TwoHand", false);
				break;
			case CBoss_Robot::BOSS_ATTACK_NORMAL_ONEHAND:
				m_pAnimatorCom->PlayAnimation(L"Attack_Normal_OneHand", false);
				break;
			case CBoss_Robot::BOSS_ATTACK_DAMAGED_TWOHAND:
				m_pAnimatorCom->PlayAnimation(L"Attack_Damaged_TwoHand", false);
				break;
			case CBoss_Robot::BOSS_ATTACK_DAMAGED_ONEHAND:
				m_pAnimatorCom->PlayAnimation(L"Attack_Damaged_OneHand", false);
				break;
			case CBoss_Robot::BOSS_HIT_NORMAL:
				m_pAnimatorCom->PlayAnimation(L"Hit_Normal", false);
				break;
			case CBoss_Robot::BOSS_HIT_DAMAGED:
				m_pAnimatorCom->PlayAnimation(L"Hit_Damaged", false);
				break;
			case CBoss_Robot::BOSS_SHIELD_NORMAL:
				m_pAnimatorCom->PlayAnimation(L"Shield_Normal", false);
				break;
			case CBoss_Robot::BOSS_SHIELD_DAMAGED:
				m_pAnimatorCom->PlayAnimation(L"Shield_Damaged", false);
				break;
			case CBoss_Robot::BOSS_TALKING:
				m_pAnimatorCom->PlayAnimation(L"Talking", false);
				break;
			}

			m_ePreState = m_eCurState;
		}
	}


	//Jonghan Test Code End
}

void CBoss_Robot::Attack(const _float& _fTimeDelta)//This Function Calling in Monster.cpp -> Update
{
	if (m_fAngle <= 0.f)
	{
		m_fAngle = 180.f;
	}
	_vec3 vPos, vPlayerPos, vCurve;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	m_fAttackTime += _fTimeDelta;
	Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform"));
	pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);

	//Look 벡터기준 회전
	_vec3 vLook, vRight;
	_matrix mRotation, mWorld;
	m_pTransformCom->Get_WorldMatrix(&mWorld);
	memcpy(&vRight, &mWorld.m[0][0], sizeof(_vec3));
	memcpy(&vLook, &mWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);
	vRight *= 30.f;
	D3DXMatrixRotationAxis(&mRotation, &vLook, D3DXToRadian(m_fAngle));
	D3DXVec3TransformNormal(&vCurve, &vRight, &mRotation);
	if (m_fAttackTime > m_fDelayTime)
	{
		Engine::Fire_Bullet(m_pGraphicDev, vPos, vPlayerPos, 5.f, Engine::CBulletManager::BULLET_MISSILE, vCurve + vPos);
		m_fAttackTime = 4.5f;
		m_fAngle -= 15.f;
	}
}

void CBoss_Robot::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Talking", m_pTextureCom[BOSS_TALKING], 13.f);

	m_pAnimatorCom->CreateAnimation(L"Idle_Normal", m_pTextureCom[BOSS_IDLE_NORMAL], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Idle_Damaged", m_pTextureCom[BOSS_IDLE_DAMAGED], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Idle_MoreDamaged", m_pTextureCom[BOSS_IDLE_MOREDAMAGED], 13.f);

	m_pAnimatorCom->CreateAnimation(L"Attack_Normal_TwoHand", m_pTextureCom[BOSS_ATTACK_NORMAL_TWOHAND], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Normal_OneHand", m_pTextureCom[BOSS_ATTACK_NORMAL_ONEHAND], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Damaged_TwoHand", m_pTextureCom[BOSS_ATTACK_DAMAGED_TWOHAND], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Damaged_OneHand", m_pTextureCom[BOSS_ATTACK_DAMAGED_ONEHAND], 13.f);

	m_pAnimatorCom->CreateAnimation(L"Hit_Normal", m_pTextureCom[BOSS_HIT_NORMAL], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Hit_Damaged", m_pTextureCom[BOSS_HIT_DAMAGED], 13.f);

	m_pAnimatorCom->CreateAnimation(L"Shield_Normal", m_pTextureCom[BOSS_SHIELD_NORMAL], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Shield_Damaged", m_pTextureCom[BOSS_SHIELD_DAMAGED], 13.f);


	m_pAnimatorCom->PlayAnimation(L"Talking", false);
}

_bool CBoss_Robot::Check_Phase()// if return true, Deal Shield. if not Deal Boss 
{
	if (CBoss_Robot::BOSSHEALTH_SHIELD == m_eCurHealthState)
		return true;

	return false;
}


void CBoss_Robot::Free()
{
	CMonster::Free();
}
