#include "pch.h"
#include "WallStage1.h"
#include "Export_Utility.h"

CWallStage1::CWallStage1(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CWallInfo(_pGraphicDev)
{
}

CWallStage1::~CWallStage1()
{
}

CWallStage1* CWallStage1::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CWallStage1* pWallStage1 = new CWallStage1(_pGraphicDev);

    if (FAILED(pWallStage1->Ready_GameObject()))
    {
        Engine::Safe_Release(pWallStage1);
        MSG_BOX("pWallStage1 Create Failed");
        return nullptr;
    }


    return pWallStage1;
}

HRESULT CWallStage1::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    return S_OK;
}

_int CWallStage1::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);;
}

void CWallStage1::LateUpdate_GameObject()
{
    Engine::CGameObject::LateUpdate_GameObject();
}

void CWallStage1::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    FAILED_CHECK_RETURN(Setup_Material(), );
    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CWallStage1::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CWallTex*>(Engine::Clone_Proto(L"Proto_WallTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_AcidWall"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

HRESULT CWallStage1::Setup_Material()
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

void CWallStage1::Setup_Position(_vec3 _vecPos)
{
    m_pTransformCom->Set_Pos(_vecPos.x, _vecPos.y, _vecPos.z);
}

void CWallStage1::Free()
{
    Engine::CGameObject::Free();
}
