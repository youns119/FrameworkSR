#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CCubeTex;
class CMissileTex;
class CTransform;
class CCollider;
END

class CMissile : public Engine::CBullet
{
private:
    explicit CMissile(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CMissile();
public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();
    virtual void Fire_Missile(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage, const _vec3& vCurvePos);

    _vec3 Bezier(_vec3 _vStartPos, _vec3 vCurvePos, _vec3 vEndPos, _float fTime);
    static CMissile* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
    HRESULT Add_Component();

private:
    Engine::CMissileTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom;
    Engine::CTransform* m_pTransformCom;
    Engine::CCollider* m_pColliderCom;

    _float m_fTimer;
    _float m_fFrame;
    _float m_fMaxFrame;
    _float m_fLinear;

    _vec3 m_vStart;
    _vec3 m_vEnd;
    _vec3 m_vCurve;
private:
    virtual void Free();

};

