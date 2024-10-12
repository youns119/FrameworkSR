#include "pch.h"
#include "../Header/Humanoid.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "../Header/Player.h"

// ±Ôºó
#include "../Header/EffectPool.h"

CHumanoid::CHumanoid(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CMonster(_pGraphicDev)
	, m_eCurState(HUMANOIDSTATE::HUMANOID_IDLE)
	, m_ePreState(HUMANOIDSTATE::HUMANOID_IDLE)
	, m_bIsFire(false)
	, m_fFireDelayTime(0.f)
	, m_fAttackTimer(0.f)
	, bColShow(false)
	, m_vPlayerLook({0.f, 0.f, 0.f})
{
	for (_int i = 0; i < HUMANOIDSTATE::HUMANOID_END; ++i)
		m_pTextureCom[i] = nullptr;

	m_fHP = 12.f;
}

CHumanoid::~CHumanoid()
{
}

void CHumanoid::Render_GameObject()
{
	if (!m_bIsRender) //Trigger
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//Jonghan Monster Change Start

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetTexture(0, nullptr);

	if (Engine::Key_Hold(DIK_B)) {
		bColShow = true;
	}
	if (Engine::Key_Hold(DIK_V)) {
		bColShow = false;
	}
	if (bColShow) {
		m_pHitBufferCom->Render_Buffer();
		m_pHeadHit->Render_Buffer();
		m_pCriticalHit->Render_Buffer();
	}
	m_pAnimatorCom->Render_Animator();
	m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CHumanoid::Damaged_By_Player(const DAMAGED_STATE& _eDamagedState, const _float& _fAttackDamage)
{
	_int iTemp(0);
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

			// ±Ôºó
		{
			CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_BloodJet", L"Com_Transform");
			static_cast<CTransform*>(pComponent)->Set_Pos(vPos + _vec3(0.f, 0.f, -0.1f));
			CGameObject* pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
			static_cast<CEffectPool*>(pGameObject)->Operate();
		}

		if (0 == iIndex % 4)
			Engine::Play_Sound(L"HeadShot_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
		else if (1 == iIndex % 4)
			Engine::Play_Sound(L"HeadShot_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
		else if (2 == iIndex % 4)
			Engine::Play_Sound(L"HeadShot_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
		else
			Engine::Play_Sound(L"HeadShot_04.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

		Changing_State(CHumanoid::HUMANOID_HEADSHOT);
		static_cast<CUIPlus*>(pUI)->Init(vPos, CUIPlus::UI_PLUS::PLUS_HEADSHOT);
		//static_cast<CUIPlus*>(pUI)->Init(vPos, (CUIPlus::UI_PLUS)iIndex);
		dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(3.f);
		break;
		case Engine::DAMAGED_STATE::DAMAGED_BULLSHOT:
			Changing_State(CHumanoid::HUMANOID_BULLSHOT);
			static_cast<CUIPlus*>(pUI)->Init(vPos, CUIPlus::UI_PLUS::PLUS_NUTSHOT);

			if (0 == iIndex % 4)
				Engine::Play_Sound(L"Death_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (1 == iIndex % 4)
				Engine::Play_Sound(L"Death_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (2 == iIndex % 4)
				Engine::Play_Sound(L"Death_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else
				Engine::Play_Sound(L"Death_04.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(3.f);
			break;
		case Engine::DAMAGED_STATE::DAMAGED_KATANA:
			Changing_State(CHumanoid::HUMANOID_KATANA);
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

			if (0 == iIndex % 4)
				Engine::Play_Sound(L"Death_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (1 == iIndex % 4)
				Engine::Play_Sound(L"Death_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (2 == iIndex % 4)
				Engine::Play_Sound(L"Death_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else
				Engine::Play_Sound(L"Death_04.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

			static_cast<CUIPlus*>(pUI)->Init(vPos, (CUIPlus::UI_PLUS)iIndex);
			dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(2.f);
			break;
		case Engine::DAMAGED_STATE::DAMAGED_PUSHSHOT:

			iTemp = _int(rand() % 8);

			if (0 == _int(iTemp % 2))
				Changing_State(CHumanoid::HUMANOID_PUSH_ONE);
			else
				Changing_State(CHumanoid::HUMANOID_PUSH_TWO);

			if (0 == iIndex % 4)
				Engine::Play_Sound(L"Death_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (1 == iIndex % 4)
				Engine::Play_Sound(L"Death_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else if (2 == iIndex % 4)
				Engine::Play_Sound(L"Death_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
			else
				Engine::Play_Sound(L"Death_04.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

			static_cast<CUIPlus*>(pUI)->Init(vPos, (CUIPlus::UI_PLUS)iIndex);
			dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(2.f);
			break;
		case Engine::DAMAGED_STATE::DAMAGED_EXECUTION:
			static_cast<CUIPlus*>(pUI)->Init(vPos, CUIPlus::UI_PLUS::PLUS_GOODBYE);
			dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(20.f);
			break;
		}
		//m_pColliderCom->SetActive(false);
		m_bIsDead = true;
	}
}


void CHumanoid::Free()
{
	CMonster::Free();
}
