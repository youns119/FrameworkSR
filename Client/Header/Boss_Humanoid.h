#pragma once
#include "Monster.h"
class CBoss_Humanoid :
    public CMonster
{
public:
	enum BOSS_HUMANOIDSTATE { BOSS_SPAWN,BOSS_IDLE,BOSS_ATTACK, BOSS_DAMAGED, BOSS_END };
	enum BOSS_POS {
		BOSS_CENTER,
		BOSS_CENTER_LEFT,
		BOSS_CENTER_LEFT_END,
		BOSS_CENTER_RIGHT,
		BOSS_CENTER_RIGHT_END,
		BOSS_UNDER_LEFT,
		BOSS_UNDER_LEFT_END,
		BOSS_UNDER_RIGHT,
		BOSS_UNDER_RIGHT_END,
		BOSS_UNDER_END_LEFT,
		BOSS_UNDER_END_LEFT_END,
		BOSS_UNDER_END_RIGHT,
		BOSS_UNDER_END_RIGHT_END,
		BOSS_UP_LEFT,
		BOSS_UP_LEFT_END,
		BOSS_UP_RIGHT,
		BOSS_UP_RIGHT_END,
		BOSS_UP_END_LEFT,
		BOSS_UP_END_LEFT_END,
		BOSS_UP_END_RIGHT,
		BOSS_UP_END_RIGHT_END,
	};
private:
    explicit CBoss_Humanoid(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CBoss_Humanoid(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
    virtual ~CBoss_Humanoid();

public:
	static CBoss_Humanoid* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	static CBoss_Humanoid* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual _bool Get_Execution(_vec3 _vLook, const _bool& _bIsDo) { return false; }
	_int Get_BossKillCount() { return m_iBossKillCount; }
	_bool Get_IsDamaged() { return m_bIsDamaged; }
public:
	virtual void Damaged_By_Player(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f);

private:
	virtual HRESULT	Add_Component();
	virtual void State_Check(); 
	virtual void Attack(const _float& _fTimeDelta);
	virtual void Set_Animation();
	void Changing_State(BOSS_HUMANOIDSTATE _eState) { m_eCurState = _eState; }
	void Change_State(); 
	_vec3 Calculate_NextPos(const _float& _fMinX, const _float& _fMaxX, const _float& _fMinY, const _float& _fMaxY);
	void Damaged_ReAction();

private:
	Engine::CTexture* m_pTextureCom[CBoss_Humanoid::BOSS_END];

	BOSS_HUMANOIDSTATE m_eCurState;
	BOSS_HUMANOIDSTATE m_ePreState;

	_bool m_bIsDamaged;
	_bool m_bIsAttack;
	_float m_fSpawnTimer;
	_uint m_iBossKillCount;

	_float m_fAttackTime;
	_float m_fDelayTime;
private:
	virtual void Free();
};

