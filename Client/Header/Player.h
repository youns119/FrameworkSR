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
		START,
		RIGHT_STATE_END
	};
	enum LEFT_STATE {
		LEFT_IDLE,
		LEFT_CHANGE,
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
	explicit CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vStartPos);
	explicit CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vStartPos, _bool _bBossStage);
	virtual ~CPlayer();

public:
	WEAPON_STATE Get_WeaponState() { return m_WeaponState; }
	RIGHT_STATE Get_RightCurrState() { return m_Right_CurState; }
	LEFT_STATE Get_LeftCurrState() { return m_Left_CurState; }
	_bool Get_LeftUse() { return m_bLeftHandUse; }
	_bool Get_Cool() { return m_fSkillCool >= 30.f; }
	_float Get_TimerHP() { return m_fTimerHP; }
	_float Get_PlayerHP() { return m_fHP; }
	_int Get_MaxAmmo() { return m_iMaxAmmo; }
	_int Get_CurrAmmo() { return m_iCurAmmo; }
	_int Get_CurrFloor() { return m_iCurrFloor; }
	_bool Get_HasItem() { return m_bIsHasItem; }
	_bool Get_Drinking() { return m_bIsDrinking; }
	_bool Get_Clear() { return m_bIsClear; }
	_bool Get_BossStage() { return m_bIsBoss; }
	void Rooting_Item(Engine::ITEM_TYPE _eItemType);

	void Set_PlayerHP(_float _fHP) { m_fHP = _fHP; }
	void Set_PlayerHP_Plus(_float _fHP) { m_fTimerHP += _fHP; }
	void Set_CurrFloor(_int _iFloor) { m_iCurrFloor = _iFloor; }
	void Set_Clear() { m_bIsClear = true; }

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vStartPos);
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vStartPos, _bool _bIsBossStage);

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
	void Damage_Terrain();//산성,용암 바닥 대미지 - 유빈
	void Speed_Terrain();// 슬라이드 타일 위에서 내려가기 - 유빈
	void SetAnimation();
	void Animation_End_Check();
	void Animation_Pos();
	void Rotate_Arms(const _bool& _bIsRecover);
	void Collide_Wall(CCollider& _pOther);

	void Calculate_TimerHP(const _float& _fTimeDelta);
	void Skill_Timer();
	void Moving_Rotate();
	void Shaking_Camera(const _float& _fTimeDelta);

	void ActionUI(_int _iActionType);

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
	_bool m_bIsTrapOn;// 산성,용암 대미지 받기 여부
	_bool m_bIsSlideOn;// 슬라이드 타일을 타고 있는지 체크
	_bool m_bIsLeft;
	_bool m_bIsRight;
	_bool m_bIsShaking;
	_bool m_bIsClear;
	_bool m_bIsBoss;
	_bool m_bIsShop;
	_float m_fShakingTimer;
	_float m_fShakingSize;
	_float m_fTrapTime; // 산성 용암 다시 대미지 입기까지 시간
	_float m_fHP;
	_float m_fTimerHP;
	_float m_fJumpPower;
	_float m_fTilePos;
	_float m_fSpeed;
	_float m_fDashSpeed;
	_float m_fCurAttackDelay;
	_float m_fMaxAttackDelay;
	_float m_fTime_Skill;
	_float m_fDamage;
	_float m_fSmileTimer;
	_float m_flinear[FINISH];
	_float m_fSkillCool;
	_float m_fSkillTime;
	_int m_iMaxAmmo;
	_int m_iCurAmmo;
	_int m_iCurrFloor;
	_vec3 m_vDefaultPos[FINISH];
	_vec3 m_vDefaultSize[FINISH];
	_vec3 m_vStartPos;

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