#pragma once
#include "Drone.h"

BEGIN(Engine)

class CTriCol;
class CRcTex;
class CTexture;
class CCalculator;
class CTransform;

END

class CDogDrone : public CDrone
{
private:
    explicit CDogDrone(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CDogDrone();

public:
    static CDogDrone* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT	Ready_GameObject();

private:
    virtual HRESULT	Add_Component();
    virtual void State_Check(); //Jonghan Change
    virtual void Attack() {}

private:
    virtual void Free();
};
