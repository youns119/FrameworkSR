#pragma once
#include "Soda.h"

class CDrink :
    public CSoda
{
private:
    explicit CDrink(LPDIRECT3DDEVICE9 _pGraphiceDev);
    virtual ~CDrink();

public:
    static CDrink* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();
    virtual void Spawn_Soda(_vec3 _vStartPos);
    virtual void Set_IsRender(const _bool& _bool);

public:
    virtual void OnCollisionEnter(CCollider& _pOther);

private:
    virtual HRESULT Add_Component();

private:
    CTexture* m_pTextureCom;
    CRcTex* m_pBufferCom;
    CTransform* m_pTransformCom;
    CCollider* m_pColliderCom;

    ITEM_TYPE m_eItemType;
    _float m_fTimer;

private:
    virtual void Free();
};

