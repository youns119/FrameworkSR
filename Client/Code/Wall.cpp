#include "pch.h"
#include "../Header/Wall.h"
#include "Export_Utility.h"

CWall::CWall(LPDIRECT3DDEVICE9 _pGraphicDev)
    : Engine::CGameObject(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pTransformCom(nullptr)
    , m_pTextureCom(nullptr)
    , m_pColliderCom(nullptr)
{
    m_pName = L"Proto_WALLCORNER01";
}

CWall::~CWall()
{
}

CWall* CWall::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CWall* pWall = new CWall(_pGraphicDev);

    if (FAILED(pWall->Ready_GameObject()))
    {
        Safe_Release(pWall);
        MSG_BOX("pWall Create Failed");
        return nullptr;
    }

    return pWall;
}

CWall* CWall::Create_Pos(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
{
    CWall* pWall = new CWall(_pGraphicDev);

    if (FAILED(pWall->Ready_GameObject()))
    {
        Safe_Release(pWall);
        MSG_BOX("pWall Create Failed");
        return nullptr;
    }

    pWall->Setup_Position(_vecPos);
    pWall->m_vecPos = _vecPos;

    return pWall;
}

CWall* CWall::Create_Rot(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot)
{
    CWall* pWall = new CWall(_pGraphicDev);

    if (FAILED(pWall->Ready_GameObject()))
    {
        Safe_Release(pWall);
        MSG_BOX("pWall Create Failed");
        return nullptr;
    }

    pWall->Setup_Position(_vecPos);
    pWall->Setup_Angle(_vecRot);
    return pWall;
}

CWall* CWall::Create_Info(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _tchar* _pName)
{
    CWall* pWall = new CWall(_pGraphicDev);

    pWall->Setup_ImageName(_pName);

    if (FAILED(pWall->Ready_GameObject()))
    {
        Safe_Release(pWall);
        MSG_BOX("pTerrain Create Failed");
        return nullptr;
    }

    pWall->Setup_Position(_vecPos);
    pWall->m_vecPos = _vecPos;

    return pWall;
}

HRESULT CWall::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    _vec3 vUp, vLook, vRight;
    m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
    m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);

    _vec3 vOffset = { vUp.x + vLook.x, vUp.y + vLook.y, vUp.z + vLook.z };

    m_pColliderCom->SetTransform(m_pTransformCom);
    m_pColliderCom->SetRadius(1.f);
    m_pColliderCom->SetOffsetPos(vOffset);
    m_pColliderCom->SetLookDir(vRight);
    m_pColliderCom->SetShow(true);
    m_pColliderCom->SetActive(true);

    return S_OK;
}

_int CWall::Update_GameObject(const _float& _fTimeDelta)
{
    Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

    Engine::Add_Collider(m_pColliderCom);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CWall::LateUpdate_GameObject()
{
    Engine::CGameObject::LateUpdate_GameObject();
}

void CWall::Render_GameObject()
{  
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    FAILED_CHECK_RETURN(Setup_Material(), );

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CWall::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CWallTex*>(Engine::Clone_Proto(L"Proto_WallTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_pName));
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

void CWall::Setup_ImageName(const _tchar* _pName)
{
    m_pName = _pName;
}
void CWall::Free()
{
    Engine::CGameObject::Free();
}
