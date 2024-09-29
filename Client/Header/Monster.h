#pragma once

#include "Character.h"

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
public:
	enum MONSTERBODY { MONSTERBODY_HEAD, MONSTERBODY_BULL, MONSTERBODY_BODY, MONSTERBODY_END };

protected:
	explicit CMonster(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMonster();

public:
	virtual HRESULT	Ready_GameObject()PURE;
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject()PURE;

public:
	virtual void Damaged(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f);
	virtual void Damaged_By_Player(MONSTERBODY _eMonsterBody = MONSTERBODY::MONSTERBODY_BODY, const _float& _fAttackDamage = 0.f) PURE;

protected:
	virtual HRESULT	Add_Component()PURE;
	virtual void State_Check()PURE; //Jonghan Change
	virtual void Attack(const _float& _fTimeDelta)PURE;
	virtual void Set_Animation()PURE;

protected:
	virtual void Free();

protected:
	//Jonghan Change Start

	//Engine::CTriCol* m_pBufferCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CCalculator* m_pCalculatorCom;
	Engine::CCollider* m_pColliderCom;
	Engine::CRcCol* m_pHitBufferCom;

	Engine::CAnimator* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;

	_bool m_bIsDead;

	//Jonghan Change End
};