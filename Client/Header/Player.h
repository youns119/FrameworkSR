#pragma once

#include "Character.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CAnimator;
class CComponentCamera;
class CCollider;

END

class CPlayer
	: public Engine::CCharacter
{
private:
	enum RIGHT_STATE {
		EXECUTION,
		IDLE,
		SHOOT,
		RELOAD,
		CHANGE,
		ZOOMIN,
		ZOOMING,
		ZOOMOUT,
		RIGHT_STATE_END
	};
	enum LEFT_STATE {
		LEFT_IDLE,
		DRINK,
		LEFT_EXECUTION,
		MINIGUN_BODY_IDLE,
		MINIGUN_BODY_CHANGE,
		LEFT_STATE_END
	};

	enum LEG_STATE {
		LEG_IDLE,
		KICK,
		SLIDING,
		MINIGUN_PANEL_CHANGE,
		MINIGUN_PANEL_IDLE,
		LEG_STATE_END
	};

	enum SELECT {
		LEFT,
		RIGHT,
		LEG,
		FINISH
	};

	enum WEAPON_STATE {
		PISTOL,
		RIFLE,
		SNIPER,
		SHOTGUN,
		KATANA,
		MINIGUN,
		WEAPON_STATE_END
	};

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CPlayer();

public:
	WEAPON_STATE Get_WeaponState() { return m_WeaponState; }
	RIGHT_STATE Get_RightCurrState() { return m_Right_CurState; }
	_float Get_PlayerHP() { return m_fHP; }
	_bool Get_HasItem() { return m_bIsHasItem; }
	_bool Get_Drinking() { return m_bIsDrinking; }
	void Rooting_Item(Engine::ITEM_TYPE _eItemType);

	void Set_PlayerHP(_float _fHP) { m_fHP = _fHP; }

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual	HRESULT Ready_GameObject();
	virtual	_int Update_GameObject(const _float& _fTimeDelta);
	virtual	void LateUpdate_GameObject();
	virtual	void Render_GameObject();

private:
	HRESULT	Add_Component();
	void Key_Input(const _float& _fTimeDelta);
	void Mouse_Move(const _float& _fTimeDelta);
	void Mouse_Fix();
	void Jump(const _float& _fTimeDelta);
	void Picking_Terrain();
	void Damage_Terrain();//산성,용암 바닥 대미지
	void SetAnimation();
	void Animation_End_Check();
	void Animation_Pos();
	void Rotate_Arms(const _bool& _bIsRecover);
	void Collide_Wall(CCollider& _pOther);

	void Calculate_TimerHP(const _float& _fTimeDelta);
	void Skill_Timer();
	void Moving_Rotate(const _float& _fTimeDelta);
public:
	void OnCollision(CCollider& _pOther);
	void OnCollisionEnter(CCollider& _pOther);
	void OnCollisionExit(CCollider& _pOther);
private:
	virtual void Free();

private:
	Engine::CCalculator* m_pCalculatorCom;

	//Beomseung
	//Buffer
	Engine::CRcTex* m_pPlayer_Buffer;
	//Transform
	Engine::CTransform* m_pBody_TransformCom;
	Engine::CTransform* m_pRight_TransformCom;
	Engine::CTransform* m_pLeft_TransformCom;
	Engine::CTransform* m_pLeg_TransformCom;
	//Texture
	Engine::CTexture* m_pRight_TextureCom[WEAPON_STATE::WEAPON_STATE_END][RIGHT_STATE::RIGHT_STATE_END];
	Engine::CTexture* m_pLeft_TextureCom[LEFT_STATE::LEFT_STATE_END];
	Engine::CTexture* m_pLeg_TextureCom[LEG_STATE::LEG_STATE_END];
	//Camera
	Engine::CComponentCamera* m_pCComponentCamera;
	//Animator
	Engine::CAnimator* m_pAnimator[FINISH];
	//Collider
	Engine::CCollider* m_pColliderCom;
private:
	_bool m_bJumpCheck;
	_bool m_bLegUse;
	_bool m_bLeftHandUse;
	_bool m_bIsHasItem;
	_bool m_bIsDrinking;
	_bool m_bIsRotation;
	_bool m_bIsTrapOn;
	_bool m_bIsLeft;
	_bool m_bIsRight;
	_float m_fMaxRotate;
	_float m_fTrapTime;
	_float m_fHP;
	_float m_fTimerHP;
	_float m_fJumpPower;
	_float m_fTilePos;
	_float m_fSpeed;
	_float m_fDashSpeed;
	_float m_fCurAttackDelay;
	_float m_fMaxAttackDelay;
	_float m_fTime_Skill;
	_float m_flinear;
	_int m_iMaxAmmo;
	_int m_iCurAmmo;
	_vec3 m_vDefaultPos[FINISH];
	_vec3 m_vDefaultSize[FINISH];

	RIGHT_STATE m_Right_CurState;
	RIGHT_STATE m_Right_PreState;

	LEFT_STATE m_Left_CurState;
	LEFT_STATE m_Left_PreState;

	LEG_STATE m_Leg_CurState;
	LEG_STATE m_Leg_PreState;

	WEAPON_STATE m_WeaponState;
	ITEM_TYPE m_eItemType;

	_float m_fFrameStart[SELECT::FINISH];
	_float m_fFrameEnd[SELECT::FINISH];
	_float m_fFrameSpeed[SELECT::FINISH];
};