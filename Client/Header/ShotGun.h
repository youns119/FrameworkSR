#pragma once
#include "Monster.h"

BEGIN(Engine)

class CTriCol;
class CRcTex;
class CTexture;
class CCalculator;
class CTransform;

END

class CShotGun : public CMonster
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

private:
    virtual void Free();
};

