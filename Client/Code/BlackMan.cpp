#include "pch.h"
#include "../Header/BlackMan.h"
#include "Export_System.h"
#include "Export_Utility.h"

CBlackMan::CBlackMan(LPDIRECT3DDEVICE9 _pGraphicDev) : 
    CHumanoid(_pGraphicDev)
{
	m_fMaxFrame = 10.f;
}

CBlackMan::~CBlackMan()
{
}

CBlackMan* CBlackMan::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CBlackMan* pMonster = new CBlackMan(_pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("BlackMan Create Failed");
		return nullptr;
	}

	return pMonster;
}

HRESULT CBlackMan::Ready_GameObject()
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

HRESULT CBlackMan::Add_Component()
{
	CComponent* pComponent = NULL;

	//pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"Proto_TriCol"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackTexture", pComponent });

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

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Calculator", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CBlackMan::State_Check()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CHumanoid::HUMANOID_ATTACK:
			m_fFrame = 0.f;
			m_fMaxFrame = 10.f;
			break;
		case CHumanoid::HUMANOID_HEADSHOT:
			m_fFrame = 0.f;
			m_fMaxFrame = 21.f;
			break;
		case CHumanoid::HUMANOID_PUSH_ONE:
			m_fFrame = 0.f;
			m_fMaxFrame = 23.f;
			break;
		case CHumanoid::HUMANOID_PUSH_TWO:
			m_fFrame = 0.f;
			m_fMaxFrame = 23.f;
			break;
		case CHumanoid::HUMANOID_BULLSHOT:
			m_fFrame = 0.f;
			m_fMaxFrame = 23.f;
			break;
		case CHumanoid::HUMANOID_SHOT_ONE:
			m_fFrame = 0.f;
			m_fMaxFrame = 24.f;
			break;
		case CHumanoid::HUMANOID_SHOT_TWO:
			m_fFrame = 0.f;
			m_fMaxFrame = 15.f;
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CBlackMan::Free()
{
	CHumanoid::Free();
}
