#include "pch.h"
#include "../Header/EffectSniperMuzzleFlash.h"
#include "Export_Utility.h"

CEffectSniperMuzzleFlash::CEffectSniperMuzzleFlash(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectSniperMuzzleFlash::~CEffectSniperMuzzleFlash()
{
}

HRESULT CEffectSniperMuzzleFlash::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_fViewZ = 20.f;
    m_pTransformCom->Set_Pos(300.f, -11.f, m_fViewZ);
    //m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
    m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
    m_pEffectCom->Set_LifeTime(0.35f);
    m_iTotalFrame = 7 * 1;

    m_pEffectCom->Set_CallBack(OnOperate);
    m_pEffectCom->Set_CallBackParam(this);

    Set_ParticleParam();

    m_pLeftTransformCom->Set_Pos(-120.f, 0.f, 0.f);
    m_pLeftTransformCom->Set_Scale(150.f, 75.f, 1.f);
    m_pRightTransformCom->Set_Pos(120.f, 0.f, 0.f);
    m_pRightTransformCom->Set_Scale(-150.f, 75.f, 1.f);

    return S_OK;
}

_int CEffectSniperMuzzleFlash::Update_GameObject(const _float& _fTimeDelta)
{
    //_vec3 vPos;
    //m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
    //ImGui::Begin("sniper muzzle flash");

    //ImGui::DragFloat3("positon", (float*)&vPos, 1.f);
    //m_pTransformCom->Set_Pos(vPos);

    //ImGui::End();

    if (!m_pEffectCom->Get_Visibility())
        return 0;

    Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectSniperMuzzleFlash::LateUpdate_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectSniperMuzzleFlash::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    _matrix matWorld, matLeft, matRight;
    matWorld = *m_pTransformCom->Get_WorldMatrix();
    matLeft = (*m_pLeftTransformCom->Get_WorldMatrix()) * matWorld;
    matRight = (*m_pRightTransformCom->Get_WorldMatrix()) * matWorld;

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pParticleTextureCom->Set_Texture();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
    m_pParticleSystemCom->Render_Parcitle();

    Animate_UV();
    m_pTextureCom->Set_Texture();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matLeft);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matRight);
    m_pBufferCom->Render_Buffer();
}

CEffectSniperMuzzleFlash* CEffectSniperMuzzleFlash::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectSniperMuzzleFlash* pEffect = new CEffectSniperMuzzleFlash(_pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("CEffec Sniper Muzzle Flash create failed");
        return nullptr;
    }

    return pEffect;
}

void CEffectSniperMuzzleFlash::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CEffectSniperMuzzleFlash::Add_Component()
{
    Engine::CComponent* pComponent(nullptr);

    pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_MuzzleFlashTexture"));
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

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

    pComponent = m_pRightTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Right", pComponent });

    pComponent = m_pLeftTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Left", pComponent });


    pComponent = m_pParticleTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_Glow_White"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Texture_Glow_White", pComponent });

    pComponent = m_pParticleSystemCom = static_cast<Engine::CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem", pComponent });

    return S_OK;
}

void CEffectSniperMuzzleFlash::Animate_UV()
{
    _float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
    m_iCurFrame = m_iTotalFrame * fPersentage;

    m_pBufferCom->Set_UV(_vec2(7.f, 1.f), m_iCurFrame);
}

void CEffectSniperMuzzleFlash::Set_ParticleParam()
{
    CParticleSystem::PARAM tParam;
    tParam.eShape = CParticleSystem::SHAPE::HEXAHEDRON;

    tParam.tInit.tHexahedron.tStartBoundary.vMax = { 0.f, 0.f, 999.f };
    tParam.tInit.tHexahedron.tStartBoundary.vMin = { 0.f, 0.f, 0.f };

    //tParam.tInit.tHexahedron.tStartBoundary.vMax = { 0.f, 0.f, 100.f };
    //tParam.tInit.tHexahedron.tStartBoundary.vMin = { 0.f, 0.f, -m_fViewZ + 5.f };
    tParam.tInit.tHexahedron.vInitVelocity = { 0.f, 0.f, 0.f };
    tParam.vVelocityNoise = { 50.f, 50.f, 0.f };
    tParam.vColor = _vec4(1.f, 225.f / 255.f, 134.f / 255.f, 0.5f);
    tParam.vColorFade = _vec4(234.f / 255.f, 89.f / 255.f, 0.f, 0.0f);
    tParam.iTotalCnt = 6;

    tParam.fSize = 100.0f;
    tParam.fSizeFade = 0.f;
    tParam.fLifeTime = 0.25f;


    m_pParticleSystemCom->Set_Parameter(tParam);
    //m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
    //m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::POINT_SCALE_DISABLE, TRUE);

    m_pParticleSystemCom->SetUp_Particle();
}

void CEffectSniperMuzzleFlash::OnOperate(void* _pParam)
{
    CEffectSniperMuzzleFlash* pThis = (CEffectSniperMuzzleFlash*)_pParam;

    pThis->m_pParticleSystemCom->Reset();
}
