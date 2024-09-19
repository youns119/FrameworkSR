#include "pch.h"
#include "../Header/Floor.h"
#include "Export_Utility.h"

CFloor::CFloor(LPDIRECT3DDEVICE9 _pGraphicDev)
    : Engine::CGameObject(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pTransformCom(nullptr)
    , m_pTextureCom(nullptr)
{
}

CFloor::~CFloor()
{
}

CFloor* CFloor::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CFloor* pFloor = new CFloor(_pGraphicDev);

    if (FAILED(pFloor->Ready_GameObject()))
    {
        Safe_Release(pFloor);
        MSG_BOX("pTerrain Create Failed");
        return nullptr;
    }

    return pFloor;
}

HRESULT CFloor::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CFloor::Update_GameObject(const _float& _fTimeDelta)
{
    Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CFloor::LateUpdate_GameObject()
{
    Engine::CGameObject::LateUpdate_GameObject();
}

void CFloor::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pTransformCom->Set_Angle(180.f, 0.f, 0.f);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
   m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

   FAILED_CHECK_RETURN(Setup_Material(), );

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
   m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFloor::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CFloorTex*>(Engine::Clone_Proto(L"Proto_FloorTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FirstFloor"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

HRESULT CFloor::Setup_Material()
{
    D3DMATERIAL9 tMtrl;
    ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

    tMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f };
    tMtrl.Specular = { 1.f, 1.f, 1.f, 1.f };
    tMtrl.Ambient = { 1.f, 1.f, 1.f, 1.f };

    tMtrl.Emissive = { 0.f, 0.f, 0.f, 0.f };
    tMtrl.Power = 0.f;

    m_pGraphicDev->SetMaterial(&tMtrl);

    return S_OK;
}

void CFloor::Free()
{
    Engine::CGameObject::Free();
}
