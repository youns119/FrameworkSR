#pragma once
#include "Humanoid.h"

class CShotGun : public CHumanoid
{
private:
    explicit CShotGun(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CShotGun();

public:
    static CShotGun* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT	Ready_GameObject();

private:
    virtual HRESULT	Add_Component();
    virtual void State_Check(); //Jonghan Change
    virtual void Attack(const _float& _fTimeDelta);
    virtual void Set_Animation();

private:
    virtual void Free();
};

