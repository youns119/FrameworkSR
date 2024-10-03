#pragma once
#include "Humanoid.h"

class CShotGun : public CHumanoid
{
private:
    explicit CShotGun(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CShotGun(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
    virtual ~CShotGun();

public:
    static CShotGun* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
    static CShotGun* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);

public:
    virtual HRESULT	Ready_GameObject();
    virtual _bool Get_Execution(_vec3 _vLook, const _bool& _bIsDo);

private:
    virtual HRESULT	Add_Component();
    virtual void State_Check(); //Jonghan Change
    virtual void Attack(const _float& _fTimeDelta);
    virtual void Set_Animation();

private:
    virtual void Free();
};

