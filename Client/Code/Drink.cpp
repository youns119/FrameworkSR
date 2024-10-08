#include "pch.h"
#include "../Header/Drink.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "../Header/Player.h"

CDrink::CDrink(LPDIRECT3DDEVICE9 _pGraphiceDev)
	: CSoda(_pGraphiceDev)
    , m_pColliderCom(nullptr)
    , m_pTransformCom(nullptr)
    , m_pTextureCom(nullptr)
    , m_pBufferCom(nullptr)
    , m_eItemType(ITEM_TYPE::ITEM_DRINK)
    , m_fTimer(0.f)
    , m_fJumpPower(0.f)
    , m_vJumpDirection(0.f, 0.f, 0.f)
    , m_bIsJump(true)
{
    m_bIsRender = false;
}

CDrink::~CDrink()
{
}

CDrink* CDrink::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CDrink* pItem = new CDrink(_pGraphicDev);

    if (FAILED(pItem->Ready_GameObject()))
    {
        Safe_Release(pItem);
        MSG_BOX("Drink Create Failed");
        return nullptr;
    }

    return pItem;
}

HRESULT CDrink::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetTransform(m_pTransformCom);
    m_pColliderCom->SetRadius(0.1f);
    m_pColliderCom->SetShow(true);
    m_pColliderCom->SetActive(true);
    _matrix matWorld;
    m_pTransformCom->Set_Scale(0.25f, 0.5f, 0.5f);
    m_pTransformCom->Set_Pos(10.5f, 0.1f, 5.f);
    m_pTransformCom->Set_Angle(D3DXToRadian(90.f), 0.f, 0.f);

    m_fJumpPower = (rand() % 10) * 0.015f;
    m_vJumpDirection = { (_float)(rand() % 5), 0.f, -(_float)(rand() % 5) };
    D3DXVec3Normalize(&m_vJumpDirection, &m_vJumpDirection);

    return S_OK;
}

_int CDrink::Update_GameObject(const _float& _fTimeDelta)
{
    if (!m_bIsRender)
        return 0;

    _int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);

    _matrix		matWorld, matView, matBill, matResult;
    m_pTransformCom->Get_WorldMatrix(&matWorld);

    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._13 = matView._13;
    matBill._31 = matView._31;
    matBill._33 = matView._33;

    matBill._11 = matView._11;
    matBill._12 = matView._12;
    matBill._21 = matView._21;
    matBill._22 = matView._22;

    matBill._22 = matView._22;
    matBill._23 = matView._23;
    matBill._32 = matView._32;
    matBill._33 = matView._33;

    D3DXMatrixInverse(&matBill, 0, &matBill);

    matResult = matBill * matWorld;

    m_pTransformCom->Set_WorldMatrix(&(matResult));

    if (m_bIsRender)
    {
        _vec3 vPos;
        m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
        m_fTimer += _fTimeDelta;
        Falling_Action(_fTimeDelta);
        if (15.f < m_fTimer)
        {
            m_fTimer = 0.f;
            Set_IsRender(false);
        }
    }

    Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

    return iExit;
}

void CDrink::LateUpdate_GameObject()
{
    if (!m_bIsRender)
        return;

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);

    Engine::CGameObject::LateUpdate_GameObject();
}

void CDrink::Render_GameObject()
{
    if (!m_bIsRender)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(); //Jonghan Change

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDrink::Spawn_Soda(_vec3 _vStartPos)
{
    m_pTransformCom->Set_Pos(_vStartPos);
}

void CDrink::Set_IsRender(const _bool& _bool)
{
    m_bIsRender = _bool;
    m_pColliderCom->SetActive(_bool);
    m_pColliderCom->SetShow(_bool);
}

void CDrink::OnCollisionEnter(CCollider& _pOther)
{
    CGameObject* pGameObject = Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player");
    dynamic_cast<CPlayer*>(pGameObject)->Rooting_Item(m_eItemType);
    m_pColliderCom->SetActive(false);
    m_bIsRender = false;
}

HRESULT CDrink::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DropDrinkTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_DropItemTexture", pComponent });

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
    pComponent->SetOwner(*this);

    return S_OK;
}

void CDrink::Falling_Action(const _float& _fTimeDelta)
{
    //떨어지고 튀어오르고 난리 부르스
    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

    if (0.25f > vPos.y && m_bIsJump)
    {
        m_fJumpPower = (rand() % 10) * 0.02f;
        m_bIsJump = false;
    }
    if (0.25f < vPos.y && !m_bIsJump)
    {
        m_bIsJump = true;
    }
    if (0.10f < vPos.y)
    {
        m_fJumpPower -= _fTimeDelta * 0.5f;
        vPos.y += m_fJumpPower;
        m_pTransformCom->Set_Pos(vPos + (m_vJumpDirection * _fTimeDelta * 2.f));
    }
}

void CDrink::Free()
{
    CSoda::Free();
}
