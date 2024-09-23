#pragma once
#include "Humanoid.h"


class CBlackMan : public CHumanoid
{
private:
    explicit CBlackMan(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CBlackMan();

public:
    static CBlackMan* Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pShield);

public:
    virtual HRESULT	Ready_GameObject();

private:
    virtual HRESULT	Add_Component();
    virtual void State_Check(); //Jonghan Change
    virtual void Attack();
    void Set_Shield(CGameObject* _pShield) { m_pShield = _pShield; }

private:
    CGameObject* m_pShield;
    _bool m_bIsShield;

private:
    virtual void Free();
};

