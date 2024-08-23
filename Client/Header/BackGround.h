#pragma once

#include "GameObject.h"

class CBackGround 
	: public Engine::CGameObject
{
private :
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackGround();

public :
	static CBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private :
	HRESULT	Add_Component();

public :
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private :
	virtual void Free();
};