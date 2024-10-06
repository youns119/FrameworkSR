#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CShield : // It'll be Spawn Only when Monster's Shield Broken
    public Engine::CGameObject
{
private:
    explicit CShield(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CShield();
public:
    static CShield* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();

public:
    _bool Get_IsRender() { return m_bIsRender; }
    void Spawn_Shield(const _vec3& _vStartPos);

private:
    HRESULT Add_Component();

private:
    Engine::CTexture* m_pTextureCom;
    Engine::CRcTex* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;

    _float m_fFrame;
    _float m_fMaxFrame;
    _float m_fFrameSpeed;


private:
    virtual void Free();
};

