#pragma once
#include "Item.h"
class CKnife :
    public CItem
{
private:
    explicit CKnife(LPDIRECT3DDEVICE9 _pGraphiceDev);
    explicit CKnife(LPDIRECT3DDEVICE9 _pGraphiceDev, _vec3 _vecPos);
    virtual ~CKnife();

public:
    static CKnife* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
    static CKnife* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);

public:
    virtual HRESULT Ready_GameObject();

public:
    virtual void OnCollisionEnter(CCollider& _pOther);

private:
    virtual HRESULT Add_Component();

private:
    virtual void Free();
};

