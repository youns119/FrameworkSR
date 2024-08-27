#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTriCol;
class CTransform;

END

class CMonster 
	: public Engine::CGameObject
{
private :
	explicit CMonster(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMonster();

public :
	static CMonster* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private :
	HRESULT	Add_Component();

private :
	virtual void Free();

private :
	Engine::CTriCol* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
};