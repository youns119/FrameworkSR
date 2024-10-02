#pragma once
#include "Item.h"
class CDrink :
    public CItem
{
private:
    explicit CDrink(LPDIRECT3DDEVICE9 _pGraphiceDev);
    virtual ~CDrink();

public:
    static CDrink* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT Ready_GameObject();

public:
    virtual void OnCollisionEnter(CCollider& _pOther);

private:
    virtual HRESULT Add_Component();

private:
    virtual void Free();
};

