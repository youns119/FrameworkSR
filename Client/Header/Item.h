#pragma once
#include "GameObject.h"
#include "../Header/Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CCollider;

END


class CItem :
    public Engine::CGameObject
{
private:
    explicit CItem(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CItem();
public:
    static CItem* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();

public:
    _bool Get_IsRender() { return m_bisRender; }

public:
    void OnCollisionEnter(CCollider& _pOther);

private:
    HRESULT Add_Component();

private:
    Engine::CTexture* m_pTextureCom;
    Engine::CRcTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CCollider* m_pColliderCom;

    _bool m_bisRender;
    Engine::ITEM_TYPE m_eItemType;

private:
    virtual void Free();
};

