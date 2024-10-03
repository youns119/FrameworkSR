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
		FINISHKILL,
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
	_bool Get_HasItem() { return m_bIsHasItem; }
	void Rooting_Item(Engine::ITEM_TYPE _eItemType) { m_eItemType = _eItemType; m_bIsHasItem = true; } //Jonghan Change

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
	void SetAnimation();
	void Animation_End_Check();
	void Animation_Pos();
	void Collide_Wall(CCollider& _pOther);
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
	_bool bJumpCheck;
	_bool bLegUse;
	_bool bLeftHandUse;
	_bool m_bIsHasItem;
	_float fJumpPower;
	_float fTilePos;
	_float fSpeed;
	_float m_fDashSpeed;
	_float flinear;
	_vec3 vDefaultPos[FINISH];
	_vec3 vDefaultSize[FINISH];

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