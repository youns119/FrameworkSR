#pragma once
#include "Item.h"
class CKnife :
    public CItem
{
private:
    explicit CKnife(LPDIRECT3DDEVICE9 _pGraphiceDev);
    virtual ~CKnife();

public:
    static CKnife* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT Ready_GameObject();

public:
    virtual void OnCollisionEnter(CCollider& _pOther);

private:
    virtual HRESULT Add_Component();

private:
    virtual void Free();
};

