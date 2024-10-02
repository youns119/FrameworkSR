#include "pch.h"
#include "../Header/Drink.h"
#include "Export_Utility.h"
#include "Export_System.h"

CDrink::CDrink(LPDIRECT3DDEVICE9 _pGraphiceDev)
	: CItem(_pGraphiceDev)
{
	m_eItemType = Engine::ITEM_TYPE::ITEM_DRINK;
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

    return S_OK;
}

void CDrink::OnCollisionEnter(CCollider& _pOther)
{
    CGameObject* pGameObject = Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player");
    dynamic_cast<CPlayer*>(pGameObject)->Rooting_Item(m_eItemType);
    m_pColliderCom->SetActive(false);
    m_bisRender = false;
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

void CDrink::Free()
{
    CItem::Free();
}
