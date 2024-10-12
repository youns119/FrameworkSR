#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CBoss_Shield :
    public CGameObject
{
private:
    explicit CBoss_Shield(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CBoss_Shield();
public:
    static CBoss_Shield* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();
public:
    void OnCollision(CCollider& _pOther);
    void OnCollisionEnter(CCollider& _pOther);
    void OnCollisionExit(CCollider& _pOther);
public:
    _bool Get_IsRender() { return m_bIsRender; }
    _float Get_Shield_HP() { return m_Shield_Hp; }
    void Set_Shield_HP(_float _pShield_HP) { m_Shield_Hp = _pShield_HP; }
    void Spawn_Shield(const _vec3& _vStartPos);

private:
    HRESULT Add_Component();

private:
    Engine::CTexture* m_pTextureCom;
    Engine::CRcTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CCollider* m_pColliderCom;

    _float m_fFrame;
    _float m_fMaxFrame;
    _float m_fFrameSpeed;
    _vec3 m_vOffset;
    _int iTest;
    _float m_Shield_Hp;

private:
    virtual void Free();
};

