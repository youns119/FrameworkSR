#include "pch.h"
#include "../Header/Wall.h"
#include "Export_Utility.h"

CWall::CWall(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CTileContorl(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pColliderCom(nullptr)
    , m_vecWallDirection({0.f, 0.f, 0.f})
{
    m_iNumber = 0;
    m_iNumber_Type = 1;
}

CWall::~CWall()
{
}

CWall* CWall::Create_InfoNumberDirectionTrigger(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber, Engine::TILE_DIRECTION _eTileDirection, const _int& _iTrigger)
{
    CWall* pWall = new CWall(_pGraphicDev);


    if (FAILED(pWall->Ready_GameObject()))
    {
        Safe_Release(pWall);
        MSG_BOX("pTerrain Create Failed");
        return nullptr;
    }

    pWall->Setup_Position(_vecPos);
    pWall->m_vecPos = _vecPos;
    pWall->Set_Number(_iNumber);
    pWall->m_iNumber = _iNumber;
    pWall->Set_TileDirection(_eTileDirection);
    pWall->Set_Trigger(_iTrigger);

    return pWall;
}

CWall* CWall::Create_InfoNumberDirectionTrigger2(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, const _int& _iNumber, const _int& _iTrigger)
{
    CWall* pWall = new CWall(_pGraphicDev);


    if (FAILED(pWall->Ready_GameObject()))
    {
        Safe_Release(pWall);
        MSG_BOX("pTerrain Create Failed");
        return nullptr;
    }

    pWall->Setup_Position(_vecPos);
    pWall->m_vecPos = _vecPos;
    pWall->Set_Number(_iNumber);
    pWall->m_iNumber = _iNumber;
    pWall->Set_TileDirection(_vecRot);
    pWall->Set_Trigger(_iTrigger);

    return pWall;
}

void CWall::Set_TileDirection(const _vec3& _vecDir)
{
    m_vecWallDirection = _vecDir;

    _vec3 vUp, vLook, vRight;
    m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
    m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);

    _vec3 vOffset =
    {
        vUp.x + vLook.x + m_vecWallDirection.x,
        vUp.y + vLook.y + m_vecWallDirection.y,
        vUp.z + vLook.z + m_vecWallDirection.z
    };

    m_pColliderCom->SetOffsetPos(vOffset);
}

void CWall::Set_TileDirection(Engine::TILE_DIRECTION _eTileDirection)
{
    switch (_eTileDirection)
    {
    case Engine::TILE_DIRECTION::TILE_FORWARD:
        m_vecWallDirection = { 0.f, 0.f, 1.f };
        break;
    case Engine::TILE_DIRECTION::TILE_RIGHT:
        m_vecWallDirection = { 1.f, 0.f, 0.f };
        break;
    case Engine::TILE_DIRECTION::TILE_LEFT:
        m_vecWallDirection = { -1.f, 0.f, 0.f };
        break;
    case Engine::TILE_DIRECTION::TILE_BACK:
        m_vecWallDirection = { 0.f, 0.f, -1.f };
        break;
    }
}

HRESULT CWall::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetTransform(m_pTransformCom);
    m_pColliderCom->SetRadius(1.f);
    m_pColliderCom->SetShow(true);
    m_pColliderCom->SetActive(true);

    return S_OK;
}

_int CWall::Update_GameObject(const _float& _fTimeDelta)
{
    if (!m_bIsRender)
        return 0;

    Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

    Engine::Add_Collider(m_pColliderCom);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CWall::LateUpdate_GameObject()
{
    if (!m_bIsRender)
        return;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CWall::Render_GameObject()
{  
    if (!m_bIsRender)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    FAILED_CHECK_RETURN(Setup_Material(), );

    m_pTextureCom->Set_Texture(m_iNumber - 1);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CWall::Set_IsRender(const _bool& _bool)
{
    m_bIsRender = _bool;
    m_pColliderCom->SetActive(_bool);
    m_pColliderCom->SetShow(_bool);
}

HRESULT CWall::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CWallTex*>(Engine::Clone_Proto(L"Proto_WallTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Wall"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
    pComponent->SetOwner(*this);

    return S_OK;
}

HRESULT CWall::Setup_Material()
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

void CWall::Setup_Position(_vec3 _vecPos)
{
    m_pTransformCom->Set_Pos(_vecPos.x, _vecPos.y, _vecPos.z);
}

void CWall::Setup_Angle(_vec3 _vecRot)
{
    m_pTransformCom->Set_Angle(_vecRot.x, _vecRot.y, _vecRot.z);
}

void CWall::Free()
{
    CTileContorl::Free();
}
