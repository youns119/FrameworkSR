#pragma once
#include "Monster.h"


class CDrone : public CMonster
{
public:
	enum DRONESTATE {DRONE_ATTACK, DRONE_IDLE, DRONE_WALK, DRONE_DAMAGED, DRONE_HEADSHOT, DRONE_END};

protected:
	explicit CDrone(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CDrone();

public:
	virtual HRESULT	Ready_GameObject()PURE;
	virtual void Render_GameObject();
	
protected:
	virtual HRESULT Add_Component()PURE;
	virtual void State_Check()PURE;

public:
	virtual void Change_State();

protected:
	Engine::CTexture* m_pTextureCom[DRONESTATE::DRONE_END];

	DRONESTATE m_eCurState;
	DRONESTATE m_ePreState;

protected:
	virtual void Free();
};

