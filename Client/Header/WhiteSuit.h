#pragma once
#include "Humanoid.h"

class CWhiteSuit : public CHumanoid
{
private:
	explicit CWhiteSuit(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CWhiteSuit();

public:
	static CWhiteSuit* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_GameObject();
	virtual _bool Get_Execution(_vec3 _vLook, const _bool& _bIsDo);

private:
	virtual HRESULT	Add_Component();
	virtual void State_Check(); //Jonghan Change
	virtual void Attack(const _float& _fTimeDelta);
	virtual void Set_Animation();

private:
	_vec3 m_vPlayerLook;

private:
	virtual void Free();
};

