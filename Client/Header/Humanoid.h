#pragma once
#include "Monster.h"
class CHumanoid : public CMonster
{
public:
	enum HUMANOIDSTATE { HUMANOID_IDLE, HUMANOID_ATTACK, HUMANOID_ATTACK_DELAY, HUMANOID_HEADSHOT, HUMANOID_PUSH_ONE, HUMANOID_PUSH_TWO, HUMANOID_BULLSHOT, HUMANOID_SHOT_ONE, HUMANOID_SHOT_TWO, HUMANOID_KATANA, HUMANOID_EXECUTION, HUMANOID_END }; //Jonghan Change

protected:
	explicit CHumanoid(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CHumanoid();

public:
	virtual HRESULT	Ready_GameObject()PURE;
	virtual void Render_GameObject();
	virtual _bool Get_Execution(_vec3 _vLook, const _bool& _bIsDo)PURE;

protected:
	virtual HRESULT Add_Component()PURE;
	virtual void State_Check()PURE;
	virtual void Attack(const _float& _fTimeDelta)PURE;
	virtual void Set_Animation()PURE;
	void Changing_State(CHumanoid::HUMANOIDSTATE _eState) { m_eCurState = _eState; }
public:
	virtual void Damaged_By_Player(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f);

protected:
	Engine::CTexture* m_pTextureCom[HUMANOIDSTATE::HUMANOID_END];

	HUMANOIDSTATE m_eCurState;
	HUMANOIDSTATE m_ePreState;

	_bool m_bIsFire;
	_float m_fFireDelayTime;
	_float m_fAttackTimer;
	_bool bColShow;
	_vec3 m_vPlayerLook;

protected:
	virtual void Free();
};

