#include "pch.h"
#include "../Header/EffectCircleLines.h"
#include "Export_Utility.h"

CEffectCircleLines::CEffectCircleLines(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectCircleLines::~CEffectCircleLines()
{
}

CEffectCircleLines* CEffectCircleLines::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectCircleLines* pEffect = new CEffectCircleLines(_pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject(), E_FAIL))
    {
        Safe_Release(pEffect);
        MSG_BOX("Effect CircleLines create Failed");
        return nullptr;
    }

    return pEffect;
}

HRESULT CEffectCircleLines::Ready_GameObject()
{
    m_fViewZ = 8.f;
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(0.f, 0.f, m_fViewZ);
    m_pTransformCom->Set_Scale((_float)WINCX / 2.f, (_float)WINCY / 2.f, 100.f);
    m_pEffectCom->Set_LifeTime(0.5f);
    m_pEffectCom->Set_Repeatable(TRUE);
    m_iTotalFrame = 6;

    return S_OK;
}

_int CEffectCircleLines::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

    // invalid compute
    //_vec3	vTemp;
    //m_pTransformCom->Get_Info(Engine::INFO::INFO_POS, &vTemp);
    //CGameObject::Compute_ViewZ(&vTemp);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectCircleLines::LateUpdate_GameObject()
{
    _float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
    m_iCurFrame = m_iTotalFrame * fPersentage;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectCircleLines::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pTextureCom->Set_Texture(m_iCurFrame);

    m_pBufferCom->Render_Buffer();
}

HRESULT CEffectCircleLines::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CircleLinesTexture"));
    //pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTex"));
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

void CEffectCircleLines::Free()
{
    Engine::CGameObject::Free();
}
