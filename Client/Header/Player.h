#pragma once

#include "GameObject.h"
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
	: public Engine::CGameObject
{
private:
	enum RIGHT_STATE {
		PISTOL_IDLE,
		PISTOL_SHOOT,
		PISTOL_RELOAD,
		PISTOL_CHANGE,
		RIFLE_IDLE,
		RIFLE_SHOOT,
		RIFLE_RELOAD,
		SHOTGUN_IDLE,
		SHOTGUN_SHOOT,
		SHOTGUN_RELOAD,
		SNIPER_IDLE,
		SNIPER_SHOOT,
		SNIPER_RELOAD,
		RIGHT_STATE_END
	};
	enum LEFT_STATE {
		LEFT_IDLE,
		DRINK,
		LEFT_STATE_END
	};

	enum LEG_STATE {
		LEG_IDLE,
		KICK,
		SLIDING,
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
		WEAPON_STATE_END
	};

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CPlayer();

public:
	WEAPON_STATE Get_WeaponState() { return m_WeaponState; }

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
	void Mouse_Move();
	void Mouse_Fix();
	void Jump(const _float& _fTimeDelta);
	void Picking_Terrain();
	void SetAnimation();
	void Animation_End_Check();
	void Animation_Pos();
	_vec3 Picking_OnTerrain();

private:
	virtual void Free();

private:
	Engine::CCalculator* m_pCalculatorCom;

	//Beomseung
	//Buffer
	Engine::CRcTex* m_pRight_BufferCom;
	Engine::CRcTex* m_pLeft_BufferCom;
	Engine::CRcTex* m_pLeg_BufferCom;
	//Transform
	Engine::CTransform* m_pBody_TransformCom;
	Engine::CTransform* m_pRight_TransformCom;
	Engine::CTransform* m_pLeft_TransformCom;
	Engine::CTransform* m_pLeg_TransformCom;
	//Texture
	Engine::CTexture* m_pRight_TextureCom[RIGHT_STATE::RIGHT_STATE_END];
	Engine::CTexture* m_pLeft_TextureCom[LEFT_STATE::LEFT_STATE_END];
	Engine::CTexture* m_pLeg_TextureCom[LEG_STATE::LEG_STATE_END];
	//Camera
	Engine::CComponentCamera* m_pCComponentCamera;
	Engine::CAnimator* m_pAnimator[FINISH];
	Engine::CCollider* m_pColliderCom;
private:
	_bool bJumpCheck;
	_bool bLegUse;
	_float fJumpPower;
	_float fTilePos;
	_float fSpeed;

	RIGHT_STATE m_Right_CurState;
	RIGHT_STATE m_Right_PreState;

	LEFT_STATE m_Left_CurState;
	LEFT_STATE m_Left_PreState;

	LEG_STATE m_Leg_CurState;
	LEG_STATE m_Leg_PreState;

	WEAPON_STATE m_WeaponState;

	_float m_fFrameStart[SELECT::FINISH];
	_float m_fFrameEnd[SELECT::FINISH];
	_float m_fFrameSpeed[SELECT::FINISH];
};