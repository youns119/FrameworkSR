#include "pch.h"
#include "../Header/FilterFundo.h"
#include "Export_Utility.h"

CFilterFundo::CFilterFundo(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CFilterFundo::~CFilterFundo()
{
}

CFilterFundo* CFilterFundo::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CFilterFundo* pFilter = new CFilterFundo(_pGraphicDev);

    if (FAILED(pFilter->Ready_GameObject(), E_FAIL))
    {
        Safe_Release(pFilter);
        MSG_BOX("Effect BossRobotBooster create Failed");
        return nullptr;
    }

    return pFilter;
}

HRESULT CFilterFundo::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(3.f, 2.f, 3.f);
    m_pTransformCom->Set_Scale(1.f, 1.f, 0.1f);
    m_fViewZ = 1000.0;
 
    return S_OK;
}

_int CFilterFundo::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

    CComponent* pComponenet = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform");
    _vec3 vPos, vLook;
    static_cast<CTransform*>(pComponenet)->Get_Info(INFO::INFO_POS, &vPos);
    static_cast<CTransform*>(pComponenet)->Get_Info(INFO::INFO_LOOK, &vLook);
    //vLook.y = 0.f;
    m_pTransformCom->Set_Pos(vPos + vLook * 0.2f);

    // ºôº¸µå

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CFilterFundo::LateUpdate_GameObject()
{
    Engine::CGameObject::LateUpdate_GameObject();
}

void CFilterFundo::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(128, 255, 255, 255));
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pTextureCom->Set_Texture(2);
	m_pBufferCom->Render_Buffer();


    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);
}

HRESULT CFilterFundo::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FUNDOTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

void CFilterFundo::Free()
{
    Engine::CGameObject::Free();
}
