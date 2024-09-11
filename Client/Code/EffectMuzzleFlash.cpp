#include "pch.h"
#include "../Header/EffectMuzzleFlash.h"
#include "Export_Utility.h"

CEffectMuzzleFlash::CEffectMuzzleFlash(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectMuzzleFlash::~CEffectMuzzleFlash()
{
}

CEffectMuzzleFlash* CEffectMuzzleFlash::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectMuzzleFlash* pMuzzleFlash = new CEffectMuzzleFlash(_pGraphicDev);

    if (FAILED(pMuzzleFlash->Ready_GameObject(), E_FAIL))
    {
        Safe_Release(pMuzzleFlash);
        MSG_BOX("Effect MuzzleFlash create Failed");
        return nullptr;
    }

    return pMuzzleFlash;
}

HRESULT CEffectMuzzleFlash::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pEffectCom->Set_LifeTime(1.f);

    return S_OK;
}

_int CEffectMuzzleFlash::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectMuzzleFlash::LateUpdate_GameObject()
{
    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectMuzzleFlash::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pTextureCom->Set_Texture();

    m_pBufferCom->Render_Buffer();
}

HRESULT CEffectMuzzleFlash::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

    return S_OK;
}

void CEffectMuzzleFlash::Free()
{
    Engine::CGameObject::Free();
}
