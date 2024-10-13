#pragma once
#include "Monster.h"


class CDrone : public CMonster
{
public:
	enum DRONESTATE {DRONE_ATTACK, DRONE_IDLE, DRONE_WALK, DRONE_DAMAGED, DRONE_HEADSHOT, DRONE_KATANA, DRONE_END};

protected:
	explicit CDrone(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CDrone();

public:
	virtual HRESULT	Ready_GameObject()PURE;
	virtual void Render_GameObject();
	virtual _bool Get_Execution(_vec3 _vLook, const _bool& _bIsDo) { return false; }
	virtual void Set_IsRender(const _bool& _bool);
	
protected:
	virtual HRESULT Add_Component()PURE;
	virtual void State_Check()PURE;
	virtual void Attack(const _float& _fTimeDelta)PURE;
	virtual void AttackMoving(const _float& _fTimeDelta, const _vec3& _vDir)PURE;
	virtual void Set_Animation()PURE;
	void Changing_State(CDrone::DRONESTATE _eState) { m_eCurState = _eState; }

public:
	virtual void Damaged_By_Player(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f);

protected:
	Engine::CTexture* m_pTextureCom[DRONESTATE::DRONE_END];

	DRONESTATE m_eCurState;
	DRONESTATE m_ePreState;

	_bool m_bIsExplosion;

protected:
	virtual void Free();
};

