#pragma once
#include "Drone.h"

BEGIN(Engine)

class CTriCol;
class CRcTex;
class CTexture;
class CCalculator;
class CTransform;

END

class CFlyingDrone : public CDrone
{
private:
    explicit CFlyingDrone(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CFlyingDrone();

public:
    static CFlyingDrone* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT	Ready_GameObject();

private:
    virtual HRESULT	Add_Component();
    virtual void State_Check(); //Jonghan Change

private:
    virtual void Free();
};

