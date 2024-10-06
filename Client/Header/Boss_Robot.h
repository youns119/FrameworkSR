#pragma once
#include "Monster.h"
class CBoss_Robot :
    public CMonster
{
public:
	enum BOSS_ROBOTSTATE {		BOSS_IDLE_NORMAL, BOSS_IDLE_DAMAGED, BOSS_IDLE_MOREDAMAGED
							  , BOSS_ATTACK_NORMAL_TWOHAND, BOSS_ATTACK_NORMAL_ONEHAND, BOSS_ATTACK_DAMAGED_TWOHAND, BOSS_ATTACK_DAMAGED_ONEHAND
							  , BOSS_HIT_NORMAL, BOSS_HIT_DAMAGED
							  , BOSS_SHIELD_NORMAL, BOSS_SHIELD_DAMAGED
							  , BOSS_TALKING
							  , BOSS_END
	}; //Talking == Spawning(or Cinematic) if you don't need it, Delete plz)
	enum BOSS_ROBOTHEALTHSTATE { BOSSHEALTH_NORMAL, BOSSHEALTH_DAMAGED, BOSSHEALTH_MOREDAMAGED, BOSSHEALTH_SHIELD, BOSSHEALTH_END };

private:
    explicit CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
    virtual ~CBoss_Robot();

public:
	_float Get_BossHP() { return m_fBoss_HP; }

public:
	static CBoss_Robot* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	static CBoss_Robot* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);

public:
	virtual HRESULT	Ready_GameObject();
	//virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual _bool Get_Execution(_vec3 _vLook, const _bool& _bIsDo) { return false; }

public:
	virtual void Damaged_By_Player(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f);

private:
	virtual HRESULT	Add_Component();
	virtual void State_Check(); //Jonghan Change
	virtual void Attack(const _float& _fTimeDelta);
	virtual void Set_Animation();
	void Changing_State(BOSS_ROBOTSTATE _eState) { m_eCurState = _eState; }
	_bool Check_Phase();

private:
	Engine::CTexture* m_pTextureCom[CBoss_Robot::BOSS_END];

	BOSS_ROBOTSTATE m_eCurState;
	BOSS_ROBOTSTATE m_ePreState;

	BOSS_ROBOTHEALTHSTATE m_eCurHealthState;

	_float m_fBoss_HP;
	_float m_fShield_HP;

	_float m_fDelayTime;
	_float m_fAttackTime;
	_float fLinear;
	_float m_fAngle;
private:
	virtual void Free();
};

