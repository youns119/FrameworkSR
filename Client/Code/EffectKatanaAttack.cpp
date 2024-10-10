#include "pch.h"
#include "../Header/EffectKatanaAttack.h"
#include "Export_Utility.h"

CEffectKatanaAttack::CEffectKatanaAttack(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectKatanaAttack::~CEffectKatanaAttack()
{
}

HRESULT CEffectKatanaAttack::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_fViewZ = 10.9f;
    m_pTransformCom->Set_Pos(0.f, 0.f, m_fViewZ);
    m_pTransformCom->Set_Scale(WINCX * 0.5f, 100.f, 1.f);

    m_pEffectCom[0]->Set_LifeTime(0.3f);
    m_pEffectCom[1]->Set_LifeTime(0.3f);
    m_pEffectCom[2]->Set_LifeTime(0.3f);

    m_pEffectCom[0]->Set_Repeatable(FALSE);
    m_pEffectCom[1]->Set_Repeatable(FALSE);
    m_pEffectCom[2]->Set_Repeatable(FALSE);

    m_pEffectCom[0]->Set_CallBack(OnOperateFirst);
    m_pEffectCom[1]->Set_CallBack(OnOperateSecond);
    m_pEffectCom[2]->Set_CallBack(OnOperateThird);
    m_pEffectCom[0]->Set_CallBackParam(this);
    m_pEffectCom[1]->Set_CallBackParam(this);
    m_pEffectCom[2]->Set_CallBackParam(this);

    m_iTotalFrame = 5 * 1;
    m_iAdditionalTotalFrame = 1 * 5;

    return S_OK;
}

_int CEffectKatanaAttack::Update_GameObject(const _float& _fTimeDelta)
{
    _bool bIsVisible(FALSE);
    for (int i = 0; i < 3; ++i)
        bIsVisible |= m_pEffectCom[i]->Get_Visibility();

    if (!bIsVisible)
        return 0;

    Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectKatanaAttack::LateUpdate_GameObject()
{
    _bool bIsVisible(FALSE);
    for (int i = 0; i < 3; ++i)
        bIsVisible |= m_pEffectCom[i]->Get_Visibility();

    if (!bIsVisible)
        return;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectKatanaAttack::Render_GameObject()
{
    _bool bIsVisible(FALSE);
    for (int i = 0; i < 3; ++i)
        bIsVisible |= m_pEffectCom[i]->Get_Visibility();

    if (!bIsVisible)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    if (m_pEffectCom[0]->Get_Visibility())
    {
        m_pTextureCom->Set_Texture(1);

        Animate_UV(0);
        m_pBufferCom->Render_Buffer();
    }

    if (m_pEffectCom[1]->Get_Visibility())
    {
        m_pTextureCom->Set_Texture(0);

        Animate_UV(1);
        m_pBufferCom->Render_Buffer();
    }

    if (m_pEffectCom[2]->Get_Visibility())
    {
        m_pTextureCom->Set_Texture(0);

        Animate_UV(2);
        m_pBufferCom->Render_Buffer();
    }
}

CEffectKatanaAttack* CEffectKatanaAttack::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectKatanaAttack* pEffect = new CEffectKatanaAttack(_pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("CEffec tKick create failed");
        return nullptr;
    }

    return pEffect;
}

void CEffectKatanaAttack::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CEffectKatanaAttack::Add_Component()
{
    Engine::CComponent* pComponent(nullptr);
    pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_KatanaAttackTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pBufferCom = static_cast<Engine::CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pEffectCom[0] = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_EffectFirst", pComponent });

    pComponent = m_pEffectCom[1] = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_EffectSecond", pComponent });

    pComponent = m_pEffectCom[2] = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_EffectThird", pComponent });

    pComponent = m_pAdditionalTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_KatanaAdditionalTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AdditionalTexture", pComponent });

    pComponent = m_pAdditionalTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_AdditionalTransform", pComponent });



    return S_OK;
}

void CEffectKatanaAttack::Animate_UV(_uint i)
{
    _float fPersentage = m_pEffectCom[i]->Get_ElapsedPersentage();
    m_iCurFrame = m_iTotalFrame * fPersentage;

    m_pBufferCom->Set_UV(_vec2(5.f, 1.f), m_iCurFrame);
}

void CEffectKatanaAttack::OnOperateFirst(void* _pParam)
{
    CEffectKatanaAttack* pThis = (CEffectKatanaAttack*)_pParam;
    pThis->m_pTransformCom->Set_Scale(WINCX * 0.8f, 100.f, 1.f);
    pThis->m_pTransformCom->Set_Pos(0, 0.f, pThis->m_fViewZ);
    pThis->m_pTransformCom->Set_Angle(0.f, 0.f, D3DXToRadian(0.f));
}

void CEffectKatanaAttack::OnOperateSecond(void* _pParam)
{
    CEffectKatanaAttack* pThis = (CEffectKatanaAttack*)_pParam;
    pThis->m_pTransformCom->Set_Scale(150.f, WINCY * 2.f, 1.f);
    pThis->m_pTransformCom->Set_Pos(50, -100.f, pThis->m_fViewZ);
    pThis->m_pTransformCom->Set_Angle(0.f, 0.f, D3DXToRadian(40.f));
}

void CEffectKatanaAttack::OnOperateThird(void* _pParam)
{
    CEffectKatanaAttack* pThis = (CEffectKatanaAttack*)_pParam;
    pThis->m_pTransformCom->Set_Scale(-100.f, WINCY * 2.f, 1.f);
    pThis->m_pTransformCom->Set_Pos(-70, -100.f, pThis->m_fViewZ);
    pThis->m_pTransformCom->Set_Angle(0.f, 0.f, D3DXToRadian(-40.f));
}
