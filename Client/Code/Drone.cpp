#include "pch.h"
#include "../Header/Drone.h"
#include "Export_Utility.h"
#include "Export_System.h" 
#include "../Header/Player.h"

CDrone::CDrone(LPDIRECT3DDEVICE9 _pGraphicDev) : 
	CMonster(_pGraphicDev)
	, m_eCurState(DRONESTATE::DRONE_IDLE)
	, m_ePreState(DRONESTATE::DRONE_IDLE)
	, m_bIsExplosion(false)
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
	if (!m_bIsRender) //Trigger
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//Jonghan Monster Change Start

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pAnimatorCom->Render_Animator();
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDrone::Set_IsRender(const _bool& _bool)
{
	if (m_bIsExplosion)
	{
		m_bIsRender = false;
		m_pColliderCom->SetActive(false);
		m_pColliderCom->SetShow(_bool);
	}
	else
	{
		m_bIsRender = _bool;
		m_pColliderCom->SetActive(_bool);
		m_pColliderCom->SetShow(_bool);
	}
	
}

void CDrone::Damaged_By_Player(const DAMAGED_STATE& _eDamagedState, const _float& _fAttackDamage)
{
	m_fHP -= _fAttackDamage;
	Engine::Play_Sound(L"Blood_01.wav", CHANNELID::SOUND_ENEMY, 0.4f);
	if (0.f >= m_fHP)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		_int iIndex = rand() % 13;

		CUI* pUI = Engine::Activate_UI(UITYPE::UI_PLUS);
		Dead_Combo();

		CGameObject* pGameObject = nullptr;
		switch (_eDamagedState)
		{
		case Engine::DAMAGED_STATE::DAMAGED_HEADSHOT:
			Changing_State(CDrone::DRONE_HEADSHOT);

			static_cast<CUIPlus*>(pUI)->Init(vPos, CUIPlus::UI_PLUS::PLUS_HEADSHOT);
			if (0 == iIndex % 4)
				Engine::Play_Sound(L"DroneDeath_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (1 == iIndex % 4)
				Engine::Play_Sound(L"DroneDeath_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (2 == iIndex % 4)
				Engine::Play_Sound(L"DroneDeath_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else
				Engine::Play_Sound(L"DroneDeath_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

			dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(3.f);
			break;
		case Engine::DAMAGED_STATE::DAMAGED_KATANA:
			Changing_State(CDrone::DRONE_KATANA);

			static_cast<CUIPlus*>(pUI)->Init(vPos, CUIPlus::UI_PLUS::PLUS_SAYONARA);
			if (0 == iIndex % 4)
				Engine::Play_Sound(L"Blood_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (1 == iIndex % 4)
				Engine::Play_Sound(L"Blood_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (2 == iIndex % 4)
				Engine::Play_Sound(L"Blood_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else
				Engine::Play_Sound(L"Blood_04.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

			dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(3.f);
			break;
		default:
			Changing_State(CDrone::DRONE_DAMAGED);

			static_cast<CUIPlus*>(pUI)->Init(vPos, (CUIPlus::UI_PLUS)iIndex);
			if (0 == iIndex % 4)
				Engine::Play_Sound(L"DroneDeath_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (1 == iIndex % 4)
				Engine::Play_Sound(L"DroneDeath_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (2 == iIndex % 4)
				Engine::Play_Sound(L"DroneDeath_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else
				Engine::Play_Sound(L"DroneDeath_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

			dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(2.f);
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
