#pragma once
#include "Item.h"
class CAxe :
    public CItem
{
private:
    explicit CAxe(LPDIRECT3DDEVICE9 _pGraphiceDev);
    explicit CAxe(LPDIRECT3DDEVICE9 _pGraphiceDev, _vec3 _vecPos);
    virtual ~CAxe();

public:
    static CAxe* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
    static CAxe* Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);

public:
    virtual HRESULT Ready_GameObject();

public:
    virtual void OnCollisionEnter(CCollider& _pOther);

private:
    virtual HRESULT Add_Component();

private:
    virtual void Free();
};

