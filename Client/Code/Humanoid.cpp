#include "pch.h"
#include "../Header/Humanoid.h"
#include "Export_System.h"
#include "Export_Utility.h"

CHumanoid::CHumanoid(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CMonster(_pGraphicDev)
	, m_eCurState(HUMANOIDSTATE::HUMANOID_ATTACK)
	, m_ePreState(HUMANOIDSTATE::HUMANOID_ATTACK)
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

	m_pTextureCom[m_eCurState]->Set_Texture((_uint)m_fFrame); //Jonghan Change


	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CHumanoid::Change_State()
{
	if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
	{
		switch (m_eCurState)
		{
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
			m_eCurState = CHumanoid::HUMANOID_ATTACK;
			break;
		}
	}
}

void CHumanoid::Free()
{
	CMonster::Free();
}
