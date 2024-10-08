#include "pch.h"
#include "../Header/EffectKick.h"
#include "Export_Utility.h"

CEffectKick::CEffectKick(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectKick::~CEffectKick()
{
}

HRESULT CEffectKick::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_fViewZ = 10.9f;
    m_pTransformCom->Set_Pos(0.f, 0.f, 3.f);
    m_pTransformCom->Set_Scale(300.f, 450.f, 1.f);
    m_pEffectCom->Set_LifeTime(0.3f);
    m_iTotalFrame = 3 * 2;

    m_pEffectCom->Set_CallBack(OnOperate);
    m_pEffectCom->Set_CallBackParam(this);

    return S_OK;
}

_int CEffectKick::Update_GameObject(const _float& _fTimeDelta)
{
    if (!m_pEffectCom->Get_Visibility())
        return 0;

    Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectKick::LateUpdate_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectKick::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    //m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pTextureCom->Set_Texture();

    Animate_UV();
    m_pBufferCom->Render_Buffer();
}

CEffectKick* CEffectKick::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectKick* pEffect = new CEffectKick(_pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("CEffec tKick create failed");
        return nullptr;
    }

    return pEffect;
}

void CEffectKick::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CEffectKick::Add_Component()
{
    Engine::CComponent* pComponent(nullptr);
    pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_KickTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pBufferCom = static_cast<Engine::CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

    return S_OK;
}

void CEffectKick::Animate_UV()
{
    _float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
    m_iCurFrame = m_iTotalFrame * fPersentage;

    m_pBufferCom->Set_UV(_vec2(3.f, 2.f), m_iCurFrame);
}

void CEffectKick::OnOperate(void* _pParam)
{
    CEffectKick* pThis = (CEffectKick*)_pParam;

    _float fAngle = rand() % 4 * D3DX_PI * 0.5;

    pThis->m_pTransformCom->Set_Angle(0.f, 0.f, fAngle);
}
