#include "pch.h"
#include "../Header/Boss_Humanoid.h"
#include "Export_System.h"
#include "Export_Utility.h"

CBoss_Humanoid::CBoss_Humanoid(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CMonster(_pGraphicDev)
	, m_eCurState(CBoss_Humanoid::BOSS_SPAWN)
	, m_ePreState(CBoss_Humanoid::BOSS_SPAWN)
	
{
	for (_int i = 0; i < CBoss_Humanoid::BOSS_END; ++i)
		m_pTextureCom[i] = nullptr;

	m_fMaxFrame = 17.f;
}

CBoss_Humanoid::~CBoss_Humanoid()
{
}

CBoss_Humanoid* CBoss_Humanoid::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CBoss_Humanoid* pGameObject = new CBoss_Humanoid(_pGraphicDev);

	if (FAILED(pGameObject->Ready_GameObject()))
	{
		Safe_Release(pGameObject);
		MSG_BOX("Boss_Humanoid Create Failed");
		return nullptr;
	}

	return pGameObject;
}

HRESULT CBoss_Humanoid::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(2.f, 0.f, 5.f);

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetShow(true);

	Set_Animation();

	return S_OK;
}

void CBoss_Humanoid::LateUpdate_GameObject()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	//CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(COMPONENTID::ID_STATIC, L"Layer_GameLogic", L"Terrain", L"Com_Buffer"));
	//NULL_CHECK(pTerrainBufferCom);
	//
	//_float fY = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ); //this is Instance code to check Monster can ride the tile

	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

	CGameObject::Compute_ViewZ(&vPos);

	Change_State(); //this is Instance code for check Animation of Monster

	State_Check();

	Engine::CGameObject::LateUpdate_GameObject();
}

void CBoss_Humanoid::Render_GameObject()
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

void CBoss_Humanoid::Change_State()
{
	if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
	{
		switch (m_eCurState)
		{
		case CBoss_Humanoid::BOSS_SPAWN:
			m_eCurState = CBoss_Humanoid::BOSS_ATTACK;
			break;
		case CBoss_Humanoid::BOSS_ATTACK:
			m_eCurState = CBoss_Humanoid::BOSS_SPAWN;
			break;
		}
		m_pAnimatorCom->GetCurrAnim()->SetFinish(false);

	}
}

HRESULT CBoss_Humanoid::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[CBoss_Humanoid::BOSS_SPAWN] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossHumanoidSpawnTex")); //Drone coming with AttackMotion
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_SpawnTexture", pComponent });

	pComponent = m_pTextureCom[CBoss_Humanoid::BOSS_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossHumanoidAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackTexture", pComponent });

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

void CBoss_Humanoid::State_Check()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CBoss_Humanoid::BOSS_SPAWN:
			m_pAnimatorCom->PlayAnimation(L"Spawn", false);
			break;
		case CBoss_Humanoid::BOSS_ATTACK:
			m_pAnimatorCom->PlayAnimation(L"Attack", true);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CBoss_Humanoid::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Spawn", m_pTextureCom[BOSS_SPAWN], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack", m_pTextureCom[BOSS_ATTACK], 13.f);

	m_pAnimatorCom->PlayAnimation(L"Spawn", false);
}

void CBoss_Humanoid::Attack(const _float& _fTimeDelta)
{
	if (CBoss_Humanoid::BOSS_ATTACK == m_eCurState)
	{
		_vec3 vPos, vPlayerPos, vDir;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Body_Transform"));
		NULL_CHECK(pPlayerTransform, -1);

		pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);

		vDir = vPlayerPos - vPos;
	}
}

void CBoss_Humanoid::Free()
{
	CMonster::Free();
}
