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

void CHumanoid::Change_State()
{
	if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
	{
		switch (m_eCurState)
		{
		case CHumanoid::HUMANOID_IDLE:
			m_eCurState = CHumanoid::HUMANOID_ATTACK;
			break;
		case CHumanoid::HUMANOID_ATTACK:
			m_eCurState = CHumanoid::HUMANOID_HEADSHOT;
			break;
		case CHumanoid::HUMANOID_HEADSHOT:
			m_eCurState = CHumanoid::HUMANOID_PUSH_ONE;
			break;
		case CHumanoid::HUMANOID_PUSH_ONE:
			m_eCurState = CHumanoid::HUMANOID_PUSH_TWO;
			break;
		case CHumanoid::HUMANOID_PUSH_TWO:
			m_eCurState = CHumanoid::HUMANOID_BULLSHOT;
			break;
		case CHumanoid::HUMANOID_BULLSHOT:
			m_eCurState = CHumanoid::HUMANOID_SHOT_ONE;
			break;
		case CHumanoid::HUMANOID_SHOT_ONE:
			m_eCurState = CHumanoid::HUMANOID_SHOT_TWO;
			break;
		case CHumanoid::HUMANOID_SHOT_TWO:
			m_eCurState = CHumanoid::HUMANOID_IDLE;
			break;
		}
		m_pAnimatorCom->GetCurrAnim()->SetFinish(false);
	}
}

void CHumanoid::Free()
{
	CMonster::Free();
}
