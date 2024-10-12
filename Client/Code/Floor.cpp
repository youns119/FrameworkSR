#include "pch.h"
#include "../Header/Floor.h"
#include "Export_Utility.h"

CFloor::CFloor(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CTileContorl(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pColliderCom(nullptr)
    , m_fDamage(0.f)
    , m_bSlidSpeed(false)
{
    m_iNumber = 0;
    m_iNumber_Type = 0;
}

CFloor::~CFloor()
{
}

CFloor* CFloor::Create_InfoNumberTrigger(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber, const _int& _iTrigger)
{
    CFloor* pFloor = new CFloor(_pGraphicDev);


    if (FAILED(pFloor->Ready_GameObject()))
    {
        Safe_Release(pFloor);
        MSG_BOX("pTerrain Create Failed");
        return nullptr;
    }

    if (_iNumber == 27 || _iNumber == 28)
    {
        pFloor->m_fDamage = 10.f;
    }
    if (_iNumber == 13)
    {
        pFloor->m_bSlidSpeed = true;
    }

    pFloor->Setup_Position(_vecPos);
    pFloor->m_vecPos = _vecPos;
    pFloor->m_iNumber = _iNumber;
    pFloor->Set_Number(_iNumber);
    pFloor->Set_Trigger(_iTrigger);

    return pFloor;
}

CFloor* CFloor::Create_InfoNumberTrigger2(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, const _int& _iNumber, const _int& _iTrigger)
{
    CFloor* pFloor = new CFloor(_pGraphicDev);


    if (FAILED(pFloor->Ready_GameObject()))
    {
        Safe_Release(pFloor);
        MSG_BOX("pTerrain Create Failed");
        return nullptr;
    }
    if (_iNumber == 27 || _iNumber == 28)
    {
        pFloor->m_fDamage = 10.f;
    }
    if (_iNumber == 13)
    {
        pFloor->m_bSlidSpeed = true;
    }

    pFloor->Setup_Position(_vecPos);
    pFloor->Setup_Rotation(_vecRot);
    pFloor->m_vecPos = _vecPos;
    pFloor->m_vecRot = _vecRot;
    pFloor->m_iNumber = _iNumber;
    pFloor->Set_Number(_iNumber);
    pFloor->Set_Trigger(_iTrigger);

    return pFloor;
}

CFloor* CFloor::Create_Info(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, _vec3 _vecScale, const _int& _iNumber, const _int& _iTrigger)
{
    CFloor* pFloor = new CFloor(_pGraphicDev);


    if (FAILED(pFloor->Ready_GameObject()))
    {
        Safe_Release(pFloor);
        MSG_BOX("pTerrain Create Failed");
        return nullptr;
    }
    if (_iNumber == 27 || _iNumber == 28)
    {
        pFloor->m_fDamage = 10.f;
    }
    if (_iNumber == 13)
    {
        pFloor->m_bSlidSpeed = true;
    }

    pFloor->Setup_Position(_vecPos);
    pFloor->m_vecPos = _vecPos;

    pFloor->Setup_Rotation(_vecRot);
    pFloor->m_vecRot = _vecRot;
    
    pFloor->Setup_Scale(_vecScale);
    pFloor->m_vecScale = _vecScale;
    
    pFloor->Set_Number(_iNumber);
    pFloor->m_iNumber = _iNumber;
    pFloor->Set_Trigger(_iTrigger);

    return pFloor;
}

HRESULT CFloor::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    _vec3 vLook, vRight;
    m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);

    _vec3 vOffset = { vLook.x + vRight.x, vLook.y + vRight.y, vLook.z + vRight.z };

    m_pColliderCom->SetTransform(m_pTransformCom);
    m_pColliderCom->SetRadius(1.f);
    m_pColliderCom->SetOffsetPos(vOffset);
    //m_pColliderCom->SetShow(true);
    m_pColliderCom->SetActive(true);

    return S_OK;
}

_int CFloor::Update_GameObject(const _float& _fTimeDelta)
{
    if (!m_bIsRender)
        return 0;

    Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

    Engine::Add_Collider(m_pColliderCom);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CFloor::LateUpdate_GameObject()
{
    if (!m_bIsRender)
        return;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CFloor::Render_GameObject()
{
    if (!m_bIsRender)
        return;

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

   FAILED_CHECK_RETURN(Setup_Material(), );

    m_pTextureCom->Set_Texture(m_iNumber - 1);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CFloor::Set_IsRender(const _bool& _bool)
{
    m_bIsRender = _bool;
    m_pColliderCom->SetActive(_bool);
    m_pColliderCom->SetShow(false);
}

HRESULT CFloor::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CFloorTex*>(Engine::Clone_Proto(L"Proto_FloorTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Floor"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ColliderCom", pComponent });
    pComponent->SetOwner(*this);

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

void CFloor::Setup_Position(_vec3 _vecPos)
{
    m_pTransformCom->Set_Pos(_vecPos.x, _vecPos.y, _vecPos.z);
}

void CFloor::Setup_Rotation(_vec3 _vecRot)
{
    m_pTransformCom->Set_Angle(_vecRot.x, _vecRot.y, _vecRot.z);
}

void CFloor::Setup_Scale(_vec3 _vecScale)
{
    m_pTransformCom->Set_Scale(_vecScale.x, _vecScale.y, _vecScale.z);
}

void CFloor::Free()
{
    CTileContorl::Free();
}
