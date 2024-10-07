#pragma once
#include "Drone.h"


class CSpiderDrone : public CDrone
{
private:
    explicit CSpiderDrone(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CSpiderDrone(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
    virtual ~CSpiderDrone();

public:
    static CSpiderDrone* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
    static CSpiderDrone* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);

public:
    virtual HRESULT	Ready_GameObject();

private:
    virtual HRESULT	Add_Component();
    virtual void State_Check(); //Jonghan Change
    virtual void Attack(const _float& _fTimeDelta);
    virtual void AttackMoving(const _float& _fTimeDelta, const _vec3& _vDir);
    virtual void Set_Animation();

private:
    virtual void Free();
};

