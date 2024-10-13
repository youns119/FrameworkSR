#include "pch.h"
#include "../Header/Boss_Robot.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "..\Header\UIRoboto.h"
#include "../Header/Boss_Shield.h"
CBoss_Robot::CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CMonster(_pGraphicDev)
	, m_eCurState(CBoss_Robot::BOSS_TALKING)
	, m_ePreState(CBoss_Robot::BOSS_TALKING)
	, m_eCurHealthState(CBoss_Robot::BOSSHEALTH_NORMAL)
	, m_fBoss_MaxHP(100.f)
	, m_fBoss_HP(100.f)
	, m_fShield_HP(0.f)
	, m_PatternDelayTime(0.f)
	, m_fPatternAttackTime(5.f)
	, m_fDelayTime(2.8f)
	, m_fAttackTime(2.0f)
	, m_fAngle(180.f)
	, m_fMissileAngle(180.f)
	, m_bPatternEnd(true)
	, m_iRandom(0)
	, m_iCount(0)
	, m_iCount2(0)
	, m_iCount3(0)
	, m_bMoveStop(false)
	, fLinear(0.f)
	, m_LaserTime(0.f)
	, m_pShield(nullptr)
	, m_bIsShield(false)
	, m_bIsDead2(false)
	, m_iSpeed(1.f)
	, m_iDeadCount(0)
{
	for (_int i = 0; i < CBoss_Robot::BOSS_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CBoss_Robot::CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
	: CMonster(_pGraphicDev)
	, m_eCurState(CBoss_Robot::BOSS_TALKING)
	, m_ePreState(CBoss_Robot::BOSS_TALKING)
	, m_eCurHealthState(CBoss_Robot::BOSSHEALTH_NORMAL)
	, m_fBoss_MaxHP(100.f)
	, m_fBoss_HP(100.f)
	, m_fShield_HP(0.f)
	, m_PatternDelayTime(0.f)
	, m_fPatternAttackTime(5.f)
	, m_fDelayTime(2.8f)
	, m_fAttackTime(2.0f)
	, m_fAngle(180.f)
	, m_fMissileAngle(180.f)
	, m_bPatternEnd(true)
	, m_iRandom(0)
	, m_iCount(0)
	, m_iCount2(0)
	, m_iCount3(0)
	, m_bMoveStop(false)
	, fLinear(0.f)
	, m_LaserTime(0.f)
	, m_pShield(nullptr)
	, m_bIsShield(false)
	, m_bIsDead2(false)
	, m_iSpeed(1.f)
	, m_iDeadCount(0)
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

CBoss_Robot* CBoss_Robot::Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pShield, _vec3 _vecPos)
{
	CBoss_Robot* pGameObject = new CBoss_Robot(_pGraphicDev, _vecPos);

	if (FAILED(pGameObject->Ready_GameObject()))
	{
		Safe_Release(pGameObject);
		MSG_BOX("Boss_Robot Create Failed");
		return nullptr;
	}
	pGameObject->Set_Shield(_pShield);
	return pGameObject;
}

HRESULT CBoss_Robot::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Set_Pos(10.f, 5.f, 25.f);
	m_pTransformCom->Set_Scale(5.f, 5.f, 5.f);

	_vec3 vDir = { 0.5f, 0.5f, 0.5f };

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(6.f);
	m_pColliderCom->SetLookDir(vDir);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	Set_Animation();
	IsBoss = true;

	return S_OK;
}

_int CBoss_Robot::Update_GameObject(const _float& _fTimeDelta)
{
	if (nullptr == m_pPlayerTransformCom)
	{
		m_pPlayerTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform"));
	}
	Boss_Dead(_fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_Collider(m_pColliderCom);

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
	if (!m_bMoveStop && !m_bIsDead2) {
		Move(_fTimeDelta);
	}
	m_PatternDelayTime += _fTimeDelta;
	_vec3 vPlayerPosition;
	m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPosition);
	if (m_PatternDelayTime >= m_fPatternAttackTime && !m_bIsDead2 && vPlayerPosition.z >= 34.f) {
		Attack(_fTimeDelta);
	}

	m_pTransformCom->Update_Component(_fTimeDelta); //충돌처리용도로 넣어봄 Jonghan

	m_pTransformCom->Set_WorldMatrix(&(matResult));
	return iExit;
}

void CBoss_Robot::LateUpdate_GameObject()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	CGameObject::Compute_ViewZ(&vPos);

	State_Check();

	Engine::CGameObject::LateUpdate_GameObject();
}

void CBoss_Robot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//Jonghan Monster Change Start


	m_pAnimatorCom->Render_Animator();
	m_pBufferCom->Render_Buffer();

}

void CBoss_Robot::Damaged_By_Player(const DAMAGED_STATE& _eDamagedState, const _float& _fAttackDamage)
{
	if (Check_Phase())
		m_fShield_HP -= _fAttackDamage;
	else
		m_fBoss_HP -= _fAttackDamage;

	static_cast<CUIRoboto*>(Engine::Get_ListUI(UITYPE::UI_ROBOTO)->front())->Boss_Hit();
	if (m_fBoss_HP <= 0.f)
	{
		m_eCurState = BOSS_DEAD;
		m_bIsDead2 = true;

		// 보스 폭발
		CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotDie", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Operate_Effect();
	}

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
			case CBoss_Robot::BOSS_DEAD:
				m_pAnimatorCom->PlayAnimation(L"Idle_Normal", false);
				break;
			}

			m_ePreState = m_eCurState;
		}
	}


	//Jonghan Test Code End
}

void CBoss_Robot::Attack(const _float& _fTimeDelta)//This Function Calling in Monster.cpp -> Update
{
	static _uint iAttackIndex = 0;
	if (m_bPatternEnd) {
		m_iRandom = rand() % 4;
		switch (iAttackIndex)
		{
		case 0:
			Engine::Play_Sound(L"RobotBoss0.wav", CHANNELID::SOUND_ENEMY, 0.5f);
			break;
		case 1:
			Engine::Play_Sound(L"RobotBoss1.wav", CHANNELID::SOUND_ENEMY, 0.5f);
			break;
		case 2:
			Engine::Play_Sound(L"RobotBoss2.wav", CHANNELID::SOUND_ENEMY, 0.5f);
			break;
		case 3:
			Engine::Play_Sound(L"RobotBoss3.wav", CHANNELID::SOUND_ENEMY, 0.5f);
			break;
		}
		++iAttackIndex;
		iAttackIndex %= 4;
		if (m_iRandom == 2)
		{
			Engine::Play_Sound(L"Boss_Minigun.wav", CHANNELID::SOUND_ENEMY, 0.5f);
		}
	}
	if (static_cast<CBoss_Shield*>(m_pShield)->Get_Shield_HP() <= 0)
	{
		m_iRandom = 3;
	}
	Pattern_Manager(_fTimeDelta, m_iRandom);
}

void CBoss_Robot::Move(const _float& _fTimeDelta)
{
	m_fAngle -= 25.f * _fTimeDelta;
	_vec3 BuildingPos = { 20.f,25.f,45.f };
	_vec3 BuildingUp = { 0.f,1.f,0.1f };
	_vec3 vRight;
	_matrix mRotate, mWorld;
	_vec3 vUpRotate;
	m_pTransformCom->Get_WorldMatrix(&mWorld);
	memcpy(&vRight, &mWorld.m[0][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);
	vRight *= 40.f;
	D3DXMatrixRotationAxis(&mRotate, &BuildingUp, D3DXToRadian(m_fAngle));
	D3DXVec3TransformNormal(&vUpRotate, &vRight, &mRotate);


	m_pTransformCom->Set_Pos(vUpRotate + BuildingPos);

	CComponent* pComponent(nullptr);
	pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Right", L"Com_Effect");
	if (pComponent)
		static_cast<CEffect*>(pComponent)->Operate_Effect();

	pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Left", L"Com_Effect");
	if (pComponent)
		static_cast<CEffect*>(pComponent)->Operate_Effect();
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

void CBoss_Robot::Pattern_Manager(const _float& _fTimeDelta, _int _iPatternNum)
{
	_vec3 vLook, vRight;
	_vec3 vPos, vPlayerPos, vCurve;
	_matrix mRotation, mWorld;
	switch (_iPatternNum)
	{
	case BOSS_MISSILE_PATTERN:
		m_bPatternEnd = false;
		m_eCurState = BOSS_ATTACK_NORMAL_ONEHAND;
		m_fAttackTime += _fTimeDelta;
		if (m_fAttackTime > m_fDelayTime)
		{
			if (m_fMissileAngle <= 0.f)
			{
				m_fMissileAngle = 180.f;
				m_bPatternEnd = true;
				m_eCurState = BOSS_IDLE_NORMAL;
				m_PatternDelayTime = 0.f;
				break;
			}
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
			m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPos);
			m_pTransformCom->Get_WorldMatrix(&mWorld);
			memcpy(&vRight, &mWorld.m[0][0], sizeof(_vec3));
			memcpy(&vLook, &mWorld.m[2][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);
			vRight *= 30.f;
			D3DXMatrixRotationAxis(&mRotation, &vLook, D3DXToRadian(m_fMissileAngle));
			D3DXVec3TransformNormal(&vCurve, &vRight, &mRotation);
			_vec3 vOffSet = { 0.f, 0.5f, 0.f };
			Engine::Fire_Bullet(m_pGraphicDev, vPos, vPlayerPos + vOffSet, 5.f, Engine::CBulletManager::BULLET_MISSILE,true ,vCurve + vPos);
			m_fAttackTime = 2.5f;
			m_fMissileAngle -= 15.f;
		}
		break;
	case BOSS_RAZER_PATTERN:
		if (Engine::Get_Bullet_Linear(CBulletManager::BULLET_LASER) >= 0.9)
		{
			m_iCount = 0.f;
			m_bPatternEnd = true;
			m_eCurState = BOSS_IDLE_NORMAL;
			m_PatternDelayTime = 0.f;
			m_bMoveStop = false;
			break;
		}
		m_bPatternEnd = false;
		m_bMoveStop = true;
		m_eCurState = BOSS_ATTACK_NORMAL_TWOHAND;
		m_fAttackTime += _fTimeDelta;
		if (m_fAttackTime > m_fDelayTime && m_iCount < 1)
		{
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
			m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPos);
			Engine::Fire_Bullet(m_pGraphicDev, vPos, vPlayerPos, 5.f, Engine::CBulletManager::BULLET_LASER);
			m_fAttackTime = 2.5f;
			m_iCount++;
		}
		break;
	case BOSS_SHOOT_PATTERN:
		if (m_iCount2 >= 10)
		{
			m_iCount2 = 0;
			m_bPatternEnd = true;
			m_eCurState = BOSS_IDLE_NORMAL;
			m_PatternDelayTime = 0.f;
			break;
		}
		m_bPatternEnd = false;
		m_eCurState = BOSS_ATTACK_NORMAL_ONEHAND;
		m_fAttackTime += _fTimeDelta;
		if (m_fAttackTime > m_fDelayTime)
		{
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
			m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPos);
			Engine::Fire_Bullet(m_pGraphicDev, vPos, vPlayerPos, 5.f, Engine::CBulletManager::BULLET_PISTOL, true);
			m_fAttackTime = 2.5f;
			m_iCount2 += 1;
		}
		break;
	case BOSS_SHIELD_PATTERN:
	{
		if (static_cast<CBoss_Shield*>(m_pShield)->Get_Shield_HP() <= 0)
		{
			m_bPatternEnd = true;
			m_eCurState = BOSS_IDLE_NORMAL;
			m_pColliderCom->SetActive(true);
			m_pColliderCom->SetShow(true);
			static_cast<CBoss_Shield*>(m_pShield)->Set_IsRender(false);
			static_cast<CBoss_Shield*>(m_pShield)->Set_Shield_HP(10.f);
			m_iCount3 = 0;
			break;
		}
		else if (m_iCount3 == 0 && static_cast<CBoss_Shield*>(m_pShield)->Get_Shield_HP() > 0.f) {
			m_eCurState = BOSS_SHIELD_NORMAL;
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
			static_cast<CBoss_Shield*>(m_pShield)->Spawn_Shield(vPos);
			m_pColliderCom->SetActive(false);
			m_pColliderCom->SetShow(false);
			m_iCount3 += 1;
		}
		break;
	}
	default:
		break;
	}

}
void CBoss_Robot::Boss_Dead(const _float& _fTimeDelta)
{
	_vec3 vPos, vPlayerPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPos);
	if (m_bIsDead2)
	{
		if (m_iDeadCount == 0)
		{
			Engine::Play_Sound(L"Boss_Dead.wav", CHANNELID::SOUND_EFFECT, 0.5f);
			m_iDeadCount++;
		}
		_vec3 vUp;
		m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
		vUp *= -1;
		m_pTransformCom->Move_Pos(&vUp, _fTimeDelta, m_iSpeed);
		if (vPos.y <= -20.f && m_iDeadCount == 1)
		{

			m_iSpeed = 0.f;
			_matrix mWorld, mRotation;
			_vec3 vRight, vLook, vCurve;
			_int iTemp = rand() % 180;
			m_pTransformCom->Get_WorldMatrix(&mWorld);
			memcpy(&vRight, &mWorld.m[0][0], sizeof(_vec3));
			memcpy(&vLook, &mWorld.m[2][0], sizeof(_vec3));
			D3DXVec3Normalize(&vRight, &vRight);
			vRight *= 30.f;
			D3DXMatrixRotationAxis(&mRotation, &vLook, D3DXToRadian(_float(iTemp)));
			D3DXVec3TransformNormal(&vCurve, &vRight, &mRotation);

			_vec3 vOffSet = { 0.f, 0.5f, 0.f };

			//Engine::Fire_Bullet(m_pGraphicDev, vPos, vPlayerPos + vOffSet, 5.f, Engine::CBulletManager::BULLET_HEAD, true, vCurve + vPos);
			m_iDeadCount += 1;
		}

		/*if (Engine::Get_Bullet_Linear(CBulletManager::BULLET_HEAD) >= 0.98)
		{
			CPlayer* m_pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));
			m_pPlayer->Set_Right_State(CPlayer::BOSS_DEAD);
		}*/
	}
}

void CBoss_Robot::Free()
{
	CMonster::Free();
}
