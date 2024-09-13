#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTriCol;
class CRcTex;
class CTexture;
class CCalculator;
class CTransform;

END

class CMonster 
	: public Engine::CGameObject
{
public:
	enum MONSTERSTATE { MONSTER_ATTACK, MONSTER_HEADSHOT, MONSTER_PUSH_ONE, MONSTER_PUSH_TWO, MONSTER_BULLSHOT, MONSTER_SHOT_ONE, MONSTER_SHOT_TWO, MONSTER_END }; //Jonghan Change
protected :
	explicit CMonster(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMonster();

public :
	static CMonster* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

public:
	void Change_State(); //Jonghan Change

protected:
	virtual HRESULT	Add_Component();
	virtual void State_Check(); //Jonghan Change

protected:
	virtual void Free();

protected:
	//Jonghan Change Start

	//Engine::CTriCol* m_pBufferCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[MONSTERSTATE::MONSTER_END];
	Engine::CCalculator* m_pCalculatorCom;

	_float m_fFrame;
	_float m_fMaxFrame;

	MONSTERSTATE m_eCurState;
	MONSTERSTATE m_ePreState;

	Engine::CTransform* m_pTransformCom;

	//Jonghan Change End
};