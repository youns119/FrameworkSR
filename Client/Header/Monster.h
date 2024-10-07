#pragma once

#include "Character.h"
#include "../Header/UIPlus.h"

BEGIN(Engine)

class CTriCol;
class CRcTex;
class CRcCol;
class CTexture;
class CCalculator;
class CTransform;
class CCollider;
class CAnimator;

END

class CMonster 
	: public Engine::CCharacter
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMonster();

public:
	virtual HRESULT	Ready_GameObject()PURE;
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject()PURE;
	virtual void Set_IsRender(const _bool& _bool);
public:
	virtual void Damaged(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f);
	virtual void Damaged_By_Player(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f) PURE;
	//넉백시킬 크기 세팅해주는 함수
	virtual void AddForce(const _float& _fPower, _vec3 _vLook, const _float& _fDamage);
	virtual _bool Get_Execution(_vec3 _vLook, const _bool& _bIsDo)PURE;
protected:
	virtual HRESULT	Add_Component()PURE;
	virtual void State_Check()PURE; //Jonghan Change
	virtual void Attack(const _float& _fTimeDelta)PURE;
	virtual void Set_Animation()PURE;
private:
	//넉백시키는 함수
	virtual void KnockBack(const _float& _fTimeDelta);
	void Picking_Terrain();
	void Collide_Wall(CCollider& _pOther);

public:
	void OnCollision(CCollider& _pOther);
	void OnCollisionEnter(CCollider& _pOther);
	void OnCollisionExit(CCollider& _pOther);
protected:
	virtual void Free();

protected:
	//Jonghan Change Start

	//Engine::CTriCol* m_pBufferCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CCalculator* m_pCalculatorCom;
	Engine::CCollider* m_pColliderCom;
	Engine::CRcCol* m_pHitBufferCom;
	Engine::CRcCol* m_pHeadHit;
	Engine::CRcCol* m_pCriticalHit;

	Engine::CAnimator* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTransform* m_pPlayerTransformCom;

	_float m_fHP;
	_bool m_bIsExecution;
	//넉백 크기
	_vec3 vKnockBackForce;
	_vec3 m_vStartPos;
	//Jonghan Change End
	_int m_iTriggerCount;//10.06
};