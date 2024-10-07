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
protected:
    explicit CItem(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CItem();
public:
    static CItem* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT Ready_GameObject() { return S_OK; }
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();

public:
    _bool Get_IsRender() { return m_bIsRender; }

public:
    void OnCollisionEnter(CCollider& _pOther) {}

protected:
    HRESULT Add_Component() { return S_OK; }

protected:
    Engine::CTexture* m_pTextureCom;
    Engine::CRcTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CCollider* m_pColliderCom;

    Engine::ITEM_TYPE m_eItemType;

    _vec3 m_vStartPos;//맵툴에서 가져올 아이템 포지션 값

protected:
    virtual void Free();
};

