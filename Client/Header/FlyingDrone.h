#pragma once
#include "Drone.h"


class CFlyingDrone : public CDrone
{
private:
    explicit CFlyingDrone(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CFlyingDrone(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
    virtual ~CFlyingDrone();

public:
    static CFlyingDrone* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
    static CFlyingDrone* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);

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

