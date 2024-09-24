#pragma once
#include "Monster.h"
class CHumanoid : public CMonster
{
public:
	enum HUMANOIDSTATE { HUMANOID_IDLE, HUMANOID_ATTACK, HUMANOID_HEADSHOT, HUMANOID_PUSH_ONE, HUMANOID_PUSH_TWO, HUMANOID_BULLSHOT, HUMANOID_SHOT_ONE, HUMANOID_SHOT_TWO, HUMANOID_SHIELDATTACK, HUMANOID_END }; //Jonghan Change

protected:
	explicit CHumanoid(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CHumanoid();

public:
	virtual HRESULT	Ready_GameObject()PURE;
	virtual void Render_GameObject();

protected:
	virtual HRESULT Add_Component()PURE;
	virtual void State_Check()PURE;
	virtual void Attack(const _float& _fTimeDelta)PURE;
	virtual void Set_Animation()PURE;
	void Changing_State(CHumanoid::HUMANOIDSTATE _eState) { m_eCurState = _eState; }
public:
	virtual void Change_State();

protected:
	Engine::CTexture* m_pTextureCom[HUMANOIDSTATE::HUMANOID_END];

	HUMANOIDSTATE m_eCurState;
	HUMANOIDSTATE m_ePreState;

	_bool m_bIsFire;
	_float m_fFireDelayTime;
	_uint m_iAttackTiming; // Check from Attack/%d

protected:
	virtual void Free();
};

