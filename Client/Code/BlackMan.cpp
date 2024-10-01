#include "pch.h"
#include "../Header/BlackMan.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "../Header/Shield.h"

CBlackMan::CBlackMan(LPDIRECT3DDEVICE9 _pGraphicDev) : 
    CHumanoid(_pGraphicDev)
	, m_pShield(nullptr)
	, m_bIsShield(true)
{
	m_fFireDelayTime = 5.f;
	m_fAttackTimer = 6.f;
	for (_int i = 0; i < SHIELDSTATE::SHIELDSTATE_END; ++i)
		m_pShieldTextureCom[i] = nullptr;
}

CBlackMan::~CBlackMan()
{
}

CBlackMan* CBlackMan::Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pShield)
{
	CBlackMan* pMonster = new CBlackMan(_pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("BlackMan Create Failed");
		return nullptr;
	}
	pMonster->Set_Shield(_pShield);
	return pMonster;
}

HRESULT CBlackMan::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(30.f, 0.f, 10.f);

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetShow(true);

	Set_Animation();
	m_pHitBufferCom->SetvOffSet({ 0.f,0.f,0.f });
	m_pHeadHit->SetvOffSet({ 0.5f,0.5f,0.f });
	m_pCriticalHit->SetvOffSet({ -0.5f,0.5f,0.f });
	return S_OK;
}

HRESULT CBlackMan::Add_Component()
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

	pComponent = m_pShieldTextureCom[SHIELDSTATE::SHIELDSTATE_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManShieldAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShieldAttackTexture", pComponent });

	pComponent = m_pShieldTextureCom[SHIELDSTATE::SHIELDSTATE_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManShieldIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShieldIdleTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_ATTACK_DELAY] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManAttackDelayTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackDelayTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_HEADSHOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManHeadShotTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HeadShotTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_BULLSHOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManBullShotTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_BullShotTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_PUSH_ONE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManPushOneTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_PushOneTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_PUSH_TWO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManPushTwoTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_PushTwoTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_SHOT_ONE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManShotOneTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShotOneTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_SHOT_TWO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManShotTwoTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShotTwoTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_KATANA] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManKatanaDownTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_KatanaTexture", pComponent });

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

void CBlackMan::State_Check()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CHumanoid::HUMANOID_IDLE:
			if (!m_bIsShield)
				m_pAnimatorCom->PlayAnimation(L"Idle", true);
			else
				m_pAnimatorCom->PlayAnimation(L"Idle_Shield", true);
			break;
		case CHumanoid::HUMANOID_ATTACK:
			if (!m_bIsShield)
				m_pAnimatorCom->PlayAnimation(L"Attack", false);
			else
				m_pAnimatorCom->PlayAnimation(L"Attack_Shield", false);
			m_bIsFire = false;
			m_fAttackTimer = 0.f;
			break;
		case CHumanoid::HUMANOID_ATTACK_DELAY:
			if (!m_bIsShield)
				m_pAnimatorCom->PlayAnimation(L"Attack_Delay", true);
			else
				m_pAnimatorCom->PlayAnimation(L"Attack_Delay_Shield", true);
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
		}

		m_ePreState = m_eCurState;
	}
}

void CBlackMan::Attack(const _float& _fTimeDelta)
{
	_vec3 vPos, vPlayerPos, vDir, vUp, vRight;

	Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform"));
	NULL_CHECK(pPlayerTransform, -1);

	pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	if (m_bIsShield)
	{
		m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
		vPos.y += 0.5f;
		vDir = vPlayerPos - vPos;
		D3DXVec3Cross(&vRight, &vUp, &vDir);
		D3DXVec3Normalize(&vRight, &vRight);
		vPos += (vRight * 0.4f);
		vDir = vPlayerPos - vPos;
	}
	else
	{
		vPos.y += 0.65f;
		vDir = vPlayerPos - vPos;
	}

	if (15.f < D3DXVec3Length(&vDir))
	{
		Changing_State(CHumanoid::HUMANOID_IDLE);
	}
	else
	{
		if (m_fAttackTimer > m_fFireDelayTime || m_eCurState == CHumanoid::HUMANOID_IDLE)
		{
			Changing_State(CHumanoid::HUMANOID_ATTACK);
			D3DXVec3Normalize(&vDir, &vDir);
			Engine::Fire_Bullet(m_pGraphicDev, vPos, vDir, 5, CBulletManager::BULLET_PISTOL);
			m_bIsFire = true;
		}
		else if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
		{
			Changing_State(CHumanoid::HUMANOID_ATTACK_DELAY);
		}
	}

	if (m_eCurState == CHumanoid::HUMANOID_ATTACK_DELAY)
	{
		m_fAttackTimer += _fTimeDelta; // check delay, Player is not far from it
	}
}

void CBlackMan::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Idle", m_pTextureCom[HUMANOID_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack", m_pTextureCom[HUMANOID_ATTACK], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Delay", m_pTextureCom[HUMANOID_ATTACK_DELAY], 13.f);
	m_pAnimatorCom->CreateAnimation(L"HeadShot", m_pTextureCom[HUMANOID_HEADSHOT], 13.f);
	m_pAnimatorCom->CreateAnimation(L"BullShot", m_pTextureCom[HUMANOID_BULLSHOT], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Push_One", m_pTextureCom[HUMANOID_PUSH_ONE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Push_Two", m_pTextureCom[HUMANOID_PUSH_TWO], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Shot_One", m_pTextureCom[HUMANOID_SHOT_ONE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Shot_Two", m_pTextureCom[HUMANOID_SHOT_TWO], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Idle_Shield", m_pShieldTextureCom[SHIELDSTATE_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Shield", m_pShieldTextureCom[SHIELDSTATE_ATTACK], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Delay_Shield", m_pShieldTextureCom[SHIELDSTATE_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Katana", m_pTextureCom[HUMANOID_KATANA], 13.f);

	m_pAnimatorCom->PlayAnimation(L"Idle_Shield", true);
}

void CBlackMan::Damaged_By_Player(const DAMAGED_STATE& _eDamagedState, const _float& _fAttackDamage)
{
	_int iTemp(0);

	if (m_bIsShield)
	{
		m_bIsShield = false;
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		dynamic_cast<CShield*>(m_pShield)->Spawn_Shield(vPos);
		if (m_eCurState == CHumanoid::HUMANOID_IDLE)
			m_pAnimatorCom->PlayAnimation(L"Idle", true);
	}
	else if (!m_bIsShield)
	{
		switch (_eDamagedState)
		{
		case Engine::DAMAGED_STATE::DAMAGED_HEADSHOT:
			Changing_State(CHumanoid::HUMANOID_HEADSHOT);
			break;
		case Engine::DAMAGED_STATE::DAMAGED_BULLSHOT:
			Changing_State(CHumanoid::HUMANOID_BULLSHOT);
			break;
		case Engine::DAMAGED_STATE::DAMAGED_KATANA:
			Changing_State(CHumanoid::HUMANOID_KATANA);
			break;
		case Engine::DAMAGED_STATE::DAMAGED_BODYSHOT:

			iTemp = _int(rand() % 64);

			if (0 == _int(iTemp % 4))
				Changing_State(CHumanoid::HUMANOID_SHOT_ONE);
			else if (1 == _int(iTemp % 4))
				Changing_State(CHumanoid::HUMANOID_SHOT_TWO);
			else if (2 == _int(iTemp % 4))
				Changing_State(CHumanoid::HUMANOID_PUSH_ONE);
			else
				Changing_State(CHumanoid::HUMANOID_PUSH_TWO);
			break;
		}
		m_bIsDead = true;
	}
}

void CBlackMan::Free()
{
	CHumanoid::Free();
}
