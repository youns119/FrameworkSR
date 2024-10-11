#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
class CCollider;
END

class CMiniGun :
    public CBullet
{
private:
    explicit CMiniGun(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CMiniGun();

public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();
    virtual void Fire_MiniGun(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage);

    static CMiniGun* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    void OnCollisionEnter(CCollider& _pOther);

private:
    HRESULT Add_Component();

private:
    Engine::CRcTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CCollider* m_pColliderCom;

    _float m_fTimer;
    _float m_fFrame;
    _float m_fMaxFrame;

private:
    virtual void Free();
};

