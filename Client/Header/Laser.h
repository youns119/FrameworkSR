#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CTexture;
class CCubeTex;
class CTransform;
class CCollider;
END


class CLaser :
    public CBullet
{
private:
    explicit CLaser(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CLaser();
public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();
    virtual void Fire_Laser(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage);

    static CLaser* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
    HRESULT Add_Component();

private:
    Engine::CCubeTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom;
    Engine::CCollider* m_pColliderCom;
    Engine::CTransform* m_pTransformPiVot;
    Engine::CTransform* m_pTransformCom;
    Engine::CTransform* m_pSecondTransformCom;

    _float m_fTimer;
    _float m_fFrame;
    _float m_fMaxFrame;
    _float m_fLinear;

    _vec3 m_vStart;
    _vec3 m_vEnd;

    _vec3 m_vLaserObj;
    _vec3 m_vLaserDir;

    _float m_fAngle;

private:
    virtual void Free();
};

