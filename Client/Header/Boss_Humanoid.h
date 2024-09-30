#pragma once
#include "Monster.h"
class CBoss_Humanoid :
    public CMonster
{
public:
	enum BOSS_HUMANOIDSTATE { BOSS_SPAWN, BOSS_ATTACK, BOSS_DAMAGED, BOSS_END };

private:
    explicit CBoss_Humanoid(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CBoss_Humanoid();

public:
	static CBoss_Humanoid* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

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
	_float m_fSpawnTimer;
	_uint m_iBossKillCount;

private:
	virtual void Free();
};

