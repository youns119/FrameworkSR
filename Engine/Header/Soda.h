#pragma once
#include "GameObject.h"
#include "Texture.h"
#include "RcTex.h"
#include "Transform.h"

BEGIN(Engine)
class ENGINE_DLL CSoda :
    public CGameObject
{
protected:
    explicit CSoda(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CSoda();

public:
    virtual HRESULT Ready_GameObject()PURE;
    virtual _int Update_GameObject(const _float& _fTimeDelta)PURE;
    virtual void LateUpdate_GameObject()PURE;
    virtual void Render_GameObject()PURE;
    virtual void Spawn_Soda(_vec3 _vStartPos) {}
    virtual void Set_IsRender(const _bool& _bool) { m_bIsRender = _bool; } //For Trigger(Tile, Monster)
protected:
    HRESULT Add_Component() { return S_OK; }



protected:
    virtual void Free();
};

END