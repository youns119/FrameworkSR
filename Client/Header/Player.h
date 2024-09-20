#pragma once

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CComponentCamera;

END

class CPlayer
	: public Engine::CGameObject
{
private:
	enum RIGHT_STATE {
		RIGHT_IDLE,
		SHOOT,
		SLIDING,
		RIGHT_STATE_END
	};
	enum LEFT_STATE {
		LEFT_IDLE,
		DRINK,
		LEFT_STATE_END
	};
	enum SELECT {
		LEFT,
		RIGHT,
		FINISH
	};

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CPlayer();

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
	void Motion_Change();
	void Move_Frame(const _float& _fTimeDelta);
	_vec3 Picking_OnTerrain();

private:
	virtual void Free();

private:
	Engine::CCalculator* m_pCalculatorCom;

	//Beomseung
	Engine::CRcTex* m_pRight_BufferCom;
	Engine::CRcTex* m_pLeft_BufferCom;
	Engine::CTransform* m_pBody_TransformCom;
	Engine::CTransform* m_pRight_TransformCom;
	Engine::CTransform* m_pLeft_TransformCom;
	Engine::CTexture* m_pRight_TextureCom[RIGHT_STATE_END];
	Engine::CTexture* m_pLeft_TextureCom[LEFT_STATE_END];
	Engine::CComponentCamera* m_pCComponentCamera;
private:
	_bool bJumpCheck;
	_float fJumpPower;
	_float fTilePos;

	RIGHT_STATE m_Right_CurState;
	RIGHT_STATE m_Right_PreState;

	LEFT_STATE m_Left_CurState;
	LEFT_STATE m_Left_PreState;

	_float m_fFrameStart[FINISH];
	_float m_fFrameEnd[FINISH];
	_float m_fFrameSpeed[FINISH];
};