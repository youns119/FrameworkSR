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
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMonster();

public:
	virtual HRESULT	Ready_GameObject()PURE;
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject()PURE;

public:
	virtual void Change_State() PURE; //Jonghan Change

protected:
	virtual HRESULT	Add_Component()PURE;
	virtual void State_Check()PURE; //Jonghan Change
	virtual void Attack()PURE;

protected:
	virtual void Free();

protected:
	//Jonghan Change Start

	//Engine::CTriCol* m_pBufferCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CCalculator* m_pCalculatorCom;

	_float m_fFrame;
	_float m_fMaxFrame;
	_float m_fFrameSpeed;

	Engine::CTransform* m_pTransformCom;

	//Jonghan Change End
};