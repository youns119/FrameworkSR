#pragma once
#include "Humanoid.h"

BEGIN(Engine)

class CTriCol;
class CRcTex;
class CTexture;
class CCalculator;
class CTransform;

END

class CBlackMan : public CHumanoid
{
private:
    explicit CBlackMan(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CBlackMan();

public:
    static CBlackMan* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT	Ready_GameObject();

private:
    virtual HRESULT	Add_Component();
    virtual void State_Check(); //Jonghan Change

private:
    virtual void Free();
};

