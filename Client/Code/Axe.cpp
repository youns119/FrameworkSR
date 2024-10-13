#include "pch.h"
#include "../Header/Axe.h"
#include "Export_Utility.h"
#include "Export_System.h"

CAxe::CAxe(LPDIRECT3DDEVICE9 _pGraphiceDev)
	: CItem(_pGraphiceDev)
{
    m_eItemType = Engine::ITEM_TYPE::ITEM_AXE;
}
CAxe::CAxe(LPDIRECT3DDEVICE9 _pGraphiceDev, _vec3 _vecPos)
	: CItem(_pGraphiceDev)
{
    m_eItemType = Engine::ITEM_TYPE::ITEM_AXE;
    m_vStartPos = _vecPos;

}

CAxe::~CAxe()
{
}

CAxe* CAxe::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CAxe* pItem = new CAxe(_pGraphicDev);

    if (FAILED(pItem->Ready_GameObject()))
    {
        Safe_Release(pItem);
        MSG_BOX("Axe Create Failed");
        return nullptr;
    }

    return pItem;
}

CAxe* CAxe::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
{
    CAxe* pItem = new CAxe(_pGraphicDev, _vecPos);

    if (FAILED(pItem->Ready_GameObject()))
    {
        Safe_Release(pItem);
        MSG_BOX("Axe Create Failed");
        return nullptr;
    }

    return pItem;
}

HRESULT CAxe::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetTransform(m_pTransformCom);
    m_pColliderCom->SetRadius(0.1f);
    m_pColliderCom->SetShow(true);
    m_pColliderCom->SetActive(true);
    _matrix matWorld;
    m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pTransformCom->Set_Pos(m_vStartPos.x, m_vStartPos.y + 0.1f, m_vStartPos.z);
    m_pTransformCom->Set_Angle(D3DXToRadian(90.f), 0.f, 0.f);

    return S_OK;
}

void CAxe::OnCollisionEnter(CCollider& _pOther)
{
    CGameObject* pGameObject = Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player");
    if (false == dynamic_cast<CPlayer*>(pGameObject)->Get_HasItem())
    {
        dynamic_cast<CPlayer*>(pGameObject)->Rooting_Item(m_eItemType);
        m_pColliderCom->SetActive(false);
        m_bIsRender = false;

        Engine::Play_Sound(L"Item_Get.wav", CHANNELID::SOUND_INTERFACE, 0.8f);
    }
}

HRESULT CAxe::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DropAxeTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_DropItemTexture", pComponent });

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
    pComponent->SetOwner(*this);

    return S_OK;
}

void CAxe::Free()
{
    CItem::Free();
}
