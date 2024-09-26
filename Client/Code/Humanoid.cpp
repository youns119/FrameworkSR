#include "pch.h"
#include "../Header/Humanoid.h"
#include "Export_System.h"
#include "Export_Utility.h"

CHumanoid::CHumanoid(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CMonster(_pGraphicDev)
	, m_eCurState(HUMANOIDSTATE::HUMANOID_IDLE)
	, m_ePreState(HUMANOIDSTATE::HUMANOID_IDLE)
	, m_bIsFire(false)
	, m_fFireDelayTime(0.f)
	, m_fAttackTimer(0.f)
	, bColShow(false)
{
	for (_int i = 0; i < HUMANOIDSTATE::HUMANOID_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CHumanoid::~CHumanoid()
{
}

void CHumanoid::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//Jonghan Monster Change Start

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetTexture(0, nullptr);

	if (Engine::Key_Hold(DIK_B)) {
		bColShow = true;
	}
	if(Engine::Key_Hold(DIK_V)) {
		bColShow = false;
	}
	if (bColShow) {
		m_pHitBufferCom->Render_Buffer();
	}
	m_pAnimatorCom->Render_Animator();
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CHumanoid::Damaged_By_Player(MONSTERBODY _eMonsterBody, const _float& _fAttackDamage)
{
	_int iTemp(0);

	switch (_eMonsterBody)
	{
	case CMonster::MONSTERBODY_HEAD:
		Changing_State(CHumanoid::HUMANOID_HEADSHOT);
		break;
	case CMonster::MONSTERBODY_BULL:
		Changing_State(CHumanoid::HUMANOID_BULLSHOT);
		break;
	case CMonster::MONSTERBODY_BODY:

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


void CHumanoid::Free()
{
	CMonster::Free();
}
