#include "pch.h"
#include "../Header/Item.h"
#include "Export_Utility.h"
#include "Export_System.h"

CItem::CItem(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
    , m_bisRender(true)
    , m_eItemType(Engine::ITEM_TYPE::ITEM_KNIFE)
    , m_pBufferCom(nullptr)
    , m_pTextureCom(nullptr)
    , m_pColliderCom(nullptr)
    , m_pTransformCom(nullptr)
{
}

CItem::~CItem()
{
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CItem* pItem = new CItem(_pGraphicDev);

    if (FAILED(pItem->Ready_GameObject()))
    {
        Safe_Release(pItem);
        MSG_BOX("Item Create Failed");
        return nullptr;
    }

    return pItem;
}

HRESULT CItem::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetTransform(m_pTransformCom);
    m_pColliderCom->SetRadius(0.1f);
    m_pColliderCom->SetShow(true);
    m_pColliderCom->SetActive(true);
    _matrix matWorld;
    m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
    m_pTransformCom->Set_Pos(5.f, 0.1f, 5.f);
    m_pTransformCom->Set_Angle(D3DXToRadian(90.f), 0.f, 0.f);

    return S_OK;
}

_int CItem::Update_GameObject(const _float& _fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);

    _matrix		matWorld, matView, matBill, matResult;
    m_pTransformCom->Get_WorldMatrix(&matWorld);

    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

    //D3DXMatrixIdentity(&matBill);

    //matBill._11 = matView._11;
    //matBill._13 = matView._13;
    //matBill._31 = matView._31;
    //matBill._33 = matView._33;

    //matBill._11 = matView._11;
    //matBill._12 = matView._12;
    //matBill._21 = matView._21;
    //matBill._22 = matView._22;

    //matBill._22 = matView._22;
    //matBill._23 = matView._23;
    //matBill._32 = matView._32;
    //matBill._33 = matView._33;

    //D3DXMatrixInverse(&matBill, 0, &matBill);
    //
    //matResult = matBill * matWorld;
    //
    //m_pTransformCom->Set_WorldMatrix(&(matResult));

    Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

    return iExit;
}

void CItem::LateUpdate_GameObject()
{
    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
    CGameObject::Compute_ViewZ(&vPos);

    Engine::CGameObject::LateUpdate_GameObject();
}

void CItem::Render_GameObject()
{
    if (m_bisRender)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

        m_pTextureCom->Set_Texture(); //Jonghan Change

        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    }
}

void CItem::OnCollisionEnter(CCollider& _pOther)
{
    m_pColliderCom->SetActive(false);
    m_bisRender = false;
    CGameObject* pGame = Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player");
    dynamic_cast<CPlayer*>(pGame)->Rooting_Item(Engine::ITEM_TYPE::ITEM_AXE);
}

HRESULT CItem::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DropKnifeTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_DropKnifeTexture", pComponent });

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
    pComponent->SetOwner(*this);

    return S_OK;
}

void CItem::Free()
{
    CGameObject::Free();
}
