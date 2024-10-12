#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CTexture;
class CCubeTex;
class CTransform;
END


class CBoss_Humanoid_Laser :
    public CBullet
{
private:
    explicit CBoss_Humanoid_Laser(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CBoss_Humanoid_Laser();
public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();
    virtual void Boss_Sniper_Laser(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos);

    _vec3 Bezier(_vec3 _vStartPos, _vec3 vCurvePos, _vec3 vEndPos, _float fTime);

    static CBoss_Humanoid_Laser* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
    HRESULT Add_Component();

private:
    Engine::CCubeTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom;

    Engine::CTransform* m_pTransformPiVot;
    Engine::CTransform* m_pTransformCom;

    _float m_fTimer;
    _float m_fFrame;
    _float m_fMaxFrame;

    _vec3 m_vStart;
    _vec3 m_vEnd;
    _vec3 m_vCurvePos;

    _vec3 m_vLaserObj;

private:
    virtual void Free();

};

