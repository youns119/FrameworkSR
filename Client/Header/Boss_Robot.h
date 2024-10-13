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
		                      , BOSS_DEAD
							  , BOSS_END
	}; //Talking == Spawning(or Cinematic) if you don't need it, Delete plz)
	enum BOSS_ROBOTHEALTHSTATE { BOSSHEALTH_NORMAL, BOSSHEALTH_DAMAGED, BOSSHEALTH_MOREDAMAGED, BOSSHEALTH_SHIELD, BOSSHEALTH_END };
	enum BOSS_PATTERN { BOSS_MISSILE_PATTERN, BOSS_RAZER_PATTERN, BOSS_SHOOT_PATTERN, BOSS_SHIELD_PATTERN,BOSS_PATTERN_END };
private:
    explicit CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
    virtual ~CBoss_Robot();

public:
	_float Get_BossMaxHP() { return m_fBoss_MaxHP; }
	_float Get_BossHP() { return m_fBoss_HP; }
	_int Get_BossDeathCount() { return m_iDeadCount; }

public:
	static CBoss_Robot* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	static CBoss_Robot* Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pShield, _vec3 _vecPos);

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual _bool Get_Execution(_vec3 _vLook, const _bool& _bIsDo) { return false; }

public:
	virtual void Damaged_By_Player(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f);
	void Set_Shield(CGameObject* _pShield) { m_pShield = _pShield; }
private:
	virtual HRESULT	Add_Component();
	virtual void State_Check(); //Jonghan Change
	virtual void Attack(const _float& _fTimeDelta);
	void Move(const _float& _fTimeDelta);
	virtual void Set_Animation();
	void Changing_State(BOSS_ROBOTSTATE _eState) { m_eCurState = _eState; }
	_bool Check_Phase();
	void Pattern_Manager(const _float& _fTimeDelta, _int _iPatternNum);
	void Boss_Dead(const _float& _fTimeDelta);
private:
	Engine::CTexture* m_pTextureCom[CBoss_Robot::BOSS_END];

	BOSS_ROBOTSTATE m_eCurState;
	BOSS_ROBOTSTATE m_ePreState;

	BOSS_ROBOTHEALTHSTATE m_eCurHealthState;

	_float m_fBoss_MaxHP;
	_float m_fBoss_HP;
	_float m_fShield_HP;

	_float m_fDelayTime;
	_float m_PatternDelayTime;
	_float m_fPatternAttackTime;
	_float m_fAttackTime;
	_float fLinear;
	_float m_fAngle;
	_float m_fMissileAngle;
	_bool m_bPatternEnd;
	_int m_iRandom;
	_int m_iCount;
	_int m_iCount2;
	_int m_iCount3;
	_float m_LaserTime;
	_bool m_bMoveStop;
	_bool m_bIsShield;
	_bool m_bIsDead2;
	_int m_iDeadCount;
	_float m_iSpeed;
private:
	CGameObject* m_pShield;
private:
	virtual void Free();
};

