#pragma once
#include "Item.h"
class CAxe :
    public CItem
{
private:
    explicit CAxe(LPDIRECT3DDEVICE9 _pGraphiceDev);
    virtual ~CAxe();

public:
    static CAxe* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT Ready_GameObject();

public:
    virtual void OnCollisionEnter(CCollider& _pOther);

private:
    virtual HRESULT Add_Component();

private:
    virtual void Free();
};

