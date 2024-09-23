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

	m_pTextureCom[m_eCurState]->Set_Texture((_uint)m_fFrame); //Jonghan Change


	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBoss_Humanoid::Change_State()
{
	if (CBoss_Humanoid::BOSS_SPAWN == m_eCurState)
	{
		if (m_fFrame > 16.5f)
		{
			m_eCurState = CBoss_Humanoid::BOSS_ATTACK;
		}
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

	return S_OK;
}

void CBoss_Humanoid::State_Check()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CBoss_Humanoid::BOSS_SPAWN:
			m_fFrame = 0.f;
			m_fMaxFrame = 17.f;
			break;
		case CBoss_Humanoid::BOSS_ATTACK:
			m_fFrame = 0.f;
			m_fMaxFrame = 21.f;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CBoss_Humanoid::Attack()
{
}

void CBoss_Humanoid::Free()
{
	CMonster::Free();
}
