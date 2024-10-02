#include "pch.h"
#include "../Header/Drone.h"
#include "Export_Utility.h"
#include "Export_System.h" 

CDrone::CDrone(LPDIRECT3DDEVICE9 _pGraphicDev) : 
	CMonster(_pGraphicDev)
	, m_eCurState(DRONESTATE::DRONE_IDLE)
	, m_ePreState(DRONESTATE::DRONE_IDLE)
{
	for (_int i = 0; i < DRONESTATE::DRONE_END; ++i)
		m_pTextureCom[i] = nullptr;

	m_fHP = 3.f;
}

CDrone::~CDrone()
{
}

void CDrone::Render_GameObject()
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

void CDrone::Damaged_By_Player(const DAMAGED_STATE& _eDamagedState, const _float& _fAttackDamage)
{
	if (0.f >= m_fHP)
	{
		switch (_eDamagedState)
		{
		case Engine::DAMAGED_STATE::DAMAGED_HEADSHOT:
			Changing_State(CDrone::DRONE_HEADSHOT);
			break;
		case Engine::DAMAGED_STATE::DAMAGED_KATANA:
			Changing_State(CDrone::DRONE_KATANA);
			break;
		default:
			Changing_State(CDrone::DRONE_DAMAGED);
			break;
		}
		m_pColliderCom->SetActive(false);
		m_bIsDead = true;
	}
}

void CDrone::Free()
{
	CMonster::Free();
}
