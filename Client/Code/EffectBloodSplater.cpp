#include "pch.h"
#include "../Header/EffectBloodSplater.h"
#include "Export_Utility.h"

CEffectBloodSplater::CEffectBloodSplater(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectBloodSplater::~CEffectBloodSplater()
{
}

HRESULT CEffectBloodSplater::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(0.f, 1.f, 3.f);
    m_pEffectCom->Set_LifeTime(0.5f);
    m_pEffectCom->Set_Billboard(TRUE);
    m_iTotalFrame = 8;

    return S_OK;
}

_int CEffectBloodSplater::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
  
    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectBloodSplater::LateUpdate_GameObject()
{
    _vec3	vTemp;
    m_pTransformCom->Get_Info(Engine::INFO::INFO_POS, &vTemp);
    CGameObject::Compute_ViewZ(&vTemp);

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectBloodSplater::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pTextureCom->Set_Texture();

    Animate_UV();
    m_pBufferCom->Render_Buffer();
}

CEffectBloodSplater* CEffectBloodSplater::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectBloodSplater* pEffect = new CEffectBloodSplater(_pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("Effect Blood Splater create failed");
        return nullptr;
    }

    return pEffect;
}

void CEffectBloodSplater::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CEffectBloodSplater::Add_Component()
{
    Engine::CComponent* pComponent(nullptr);
    pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_BloodSplaterTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pBufferCom = static_cast<Engine::CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

    return S_OK;
}

void CEffectBloodSplater::Animate_UV()
{
    _float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
    m_iCurFrame = m_iTotalFrame * fPersentage;

    m_pBufferCom->Set_UV(_vec2(4.f, 2.f), m_iCurFrame);
}
