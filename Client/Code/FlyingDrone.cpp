#include "pch.h"
#include "../Header/FlyingDrone.h"
#include "Export_System.h"
#include "Export_Utility.h"

CFlyingDrone::CFlyingDrone(LPDIRECT3DDEVICE9 _pGraphicDev) : 
	CDrone(_pGraphicDev)
{
	m_fMaxFrame = 4.f;
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


	//m_pTransformCom->Set_Pos(10.f, 0.f, 10.f); 

	_matrix matWorld;

	m_pTransformCom->Get_WorldMatrix(&matWorld);

	matWorld._41 = 0.f;
	matWorld._43 = 10.f;

	m_pTransformCom->Set_WorldMatrix(&matWorld);

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

	return S_OK;
}

void CFlyingDrone::State_Check()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CDrone::DRONE_ATTACK:
			m_fFrame = 0.f;
			m_fMaxFrame = 4.f;
			break;
		case CDrone::DRONE_IDLE:
			m_fFrame = 0.f;
			m_fMaxFrame = 12.f;
			break;
		case CDrone::DRONE_WALK:
			m_fFrame = 0.f;
			m_fMaxFrame = 4.f;
			break;
		case CDrone::DRONE_DAMAGED:
			m_fFrame = 0.f;
			m_fMaxFrame = 4.f;
			break;
		case CDrone::DRONE_HEADSHOT:
			m_fFrame = 0.f;
			m_fMaxFrame = 4.f;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CFlyingDrone::Free()
{
	CDrone::Free();
}
