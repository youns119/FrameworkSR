#include "pch.h"
#include "../Header/GuideTerrain.h"
#include "Export_Utility.h"

CGuideTerrain::CGuideTerrain(LPDIRECT3DDEVICE9 _pGraphicDev)
    : Engine::CGameObject(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pTransformCom(nullptr)
    //, m_pTextureCom(nullptr)
{
}

CGuideTerrain::~CGuideTerrain()
{
}

CGuideTerrain* CGuideTerrain::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CGuideTerrain* pGuideTerrain = new CGuideTerrain(_pGraphicDev);

    if (FAILED(pGuideTerrain->Ready_GameObject()))
    {
        Safe_Release(pGuideTerrain);
        MSG_BOX("pTerrain Create Failed");
        return nullptr;
    }

    return pGuideTerrain;
}

HRESULT CGuideTerrain::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CGuideTerrain::Update_GameObject(const _float& _fTimeDelta)
{
    Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CGuideTerrain::LateUpdate_GameObject()
{
    Engine::CGameObject::LateUpdate_GameObject();
}

void CGuideTerrain::Render_GameObject()
{
    DWORD prestate;

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->GetRenderState(D3DRS_FILLMODE, &prestate); // 와이어 프레임 이전 상태 저장

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // 와이어 프레임 적용

    FAILED_CHECK_RETURN(Setup_Material(), );
    //m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, prestate); // 와이어 프레임 이전 상태로 다시 적용
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CGuideTerrain::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CGuideTex*>(Engine::Clone_Proto(L"Proto_GuideTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

HRESULT CGuideTerrain::Setup_Material()
{
    D3DMATERIAL9 tMtrl;
    ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

    tMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f };
    tMtrl.Specular = { 1.f, 1.f, 1.f, 1.f };
    tMtrl.Ambient = { 0.2f, 0.2f, 0.2f, 1.f };

    tMtrl.Emissive = { 0.f, 0.f, 0.f, 0.f };
    tMtrl.Power = 0.f;

    m_pGraphicDev->SetMaterial(&tMtrl);

    return S_OK;
}

void CGuideTerrain::Free()
{
    Engine::CGameObject::Free();
}
