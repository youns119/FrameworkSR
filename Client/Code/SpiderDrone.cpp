#include "pch.h"
#include "../Header/SpiderDrone.h"
#include "Export_System.h"
#include "Export_Utility.h"

CSpiderDrone::CSpiderDrone(LPDIRECT3DDEVICE9 _pGraphicDev) : 
    CDrone(_pGraphicDev)
{
	m_fMaxFrame = 5.f;
}

CSpiderDrone::~CSpiderDrone()
{
}

CSpiderDrone* CSpiderDrone::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CSpiderDrone* pMonster = new CSpiderDrone(_pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("SpiderDrone Create Failed");
		return nullptr;
	}

	return pMonster;
}

HRESULT CSpiderDrone::Ready_GameObject()
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

HRESULT CSpiderDrone::Add_Component()
{
	CComponent* pComponent = NULL;

	//pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"Proto_TriCol"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SpiderDroneAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_HEADSHOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SpiderDroneDamagedTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HeadShotTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SpiderDroneIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_WALK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SpiderDroneWalkTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_WalkTexture", pComponent });

	pComponent = m_pTextureCom[DRONESTATE::DRONE_DAMAGED] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SpiderDroneDamagedTex"));
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

void CSpiderDrone::State_Check()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CDrone::DRONE_ATTACK:
			m_fFrame = 0.f;
			m_fMaxFrame = 5.f;
			break;
		case CDrone::DRONE_IDLE:
			m_fFrame = 0.f;
			m_fMaxFrame = 8.f;
			break;
		case CDrone::DRONE_WALK:
			m_fFrame = 0.f;
			m_fMaxFrame = 10.f;
			break;
		case CDrone::DRONE_DAMAGED:
			m_fFrame = 0.f;
			m_fMaxFrame = 6.f;
			break;
		case CDrone::DRONE_HEADSHOT:
			m_fFrame = 0.f;
			m_fMaxFrame = 6.f;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CSpiderDrone::Free()
{
    CDrone::Free();
}
