#pragma once
#include "Drone.h"

BEGIN(Engine)

class CTriCol;
class CRcTex;
class CTexture;
class CCalculator;
class CTransform;

END

class CSpiderDrone : public CDrone
{
private:
    explicit CSpiderDrone(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CSpiderDrone();

public:
    static CSpiderDrone* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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

