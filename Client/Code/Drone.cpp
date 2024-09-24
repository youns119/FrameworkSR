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

void CDrone::Change_State()
{
	if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
	{
		switch (m_eCurState)
		{
		case CDrone::DRONE_ATTACK:
			m_eCurState = CDrone::DRONE_IDLE;
			break;
		case CDrone::DRONE_IDLE:
			m_eCurState = CDrone::DRONE_WALK;
			break;
		case CDrone::DRONE_WALK:
			m_eCurState = CDrone::DRONE_DAMAGED;
			break;
		case CDrone::DRONE_DAMAGED:
			m_eCurState = CDrone::DRONE_HEADSHOT;
			break;
		case CDrone::DRONE_HEADSHOT:
			m_eCurState = CDrone::DRONE_ATTACK;
			break;
		}
		m_pAnimatorCom->GetCurrAnim()->SetFinish(false);

	}
}

void CDrone::Free()
{
	CMonster::Free();
}
