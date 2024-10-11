#include "pch.h"
#include "../Header/EffectSmokeTrail.h"
#include "Export_Utility.h"
#include "Missile.h"

CEffectSmokeTrail::CEffectSmokeTrail(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectSmokeTrail::~CEffectSmokeTrail()
{
}

HRESULT CEffectSmokeTrail::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pEffectCom->Set_LifeTime(1000.f);
    m_pEffectCom->Set_CallBack(OnOperate);
    m_pEffectCom->Set_CallBackParam(this);

    Set_ParticleParameter();

    return S_OK;
}

_int CEffectSmokeTrail::Update_GameObject(const _float& _fTimeDelta)
{
    if (!m_pEffectCom->Get_Visibility())
        return 0;

    if (m_bIsMissing)
        m_fGraceTime += _fTimeDelta;

    Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectSmokeTrail::LateUpdate_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    //// 미사일 위치 받아오기 근데 누구 어떤 미사일인지 알 수 있나?
    //Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_?", L"Object", L"Com_Transform");

    ////m_pParticleSystemCom->Get_Parameter();
    //// set particle position (not particle system position)
    //// 
    //이펙트가 미사일의 트랜스폼을 받아오는 방법 : 오브젝트 풀에서 호출할때 미사일의 주소나, 미사일 트랜스폼의 주소를 넘겨야한다. 

    //CGameObject* pTarget = m_pEffectCom->Get_CallerObject();
    CMissile* pTarget = static_cast<CMissile*>(m_pEffectCom->Get_CallerObject());

    //if (nullptr == pTarget)
    if (!pTarget->Get_IsRender())
        m_bIsMissing = TRUE;

    if (m_fGraceTime >= 1.f)
        m_pEffectCom->Stop_Effect();
    else
    {
        // 미사일 트랜스폼 받아야 하는데 우선 확인을 위해 플레이어 틀ㄴ스폼 받아올겟음
        CTransform* pTargetTransform = static_cast<CTransform*>(pTarget->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
        //CTransform* pTargetTransform = static_cast<CTransform*>(pTarget->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Body_Transform"));

        _vec3 vTargetPos;
        pTargetTransform->Get_Info(INFO::INFO_POS, &vTargetPos);

        Engine::CParticleSystem::PARAM tParticleParam = m_pParticleSystemCom->Get_Parameter();
        tParticleParam.tInit.tHexahedron.tStartBoundary.vMin = vTargetPos;
        tParticleParam.tInit.tHexahedron.tStartBoundary.vMax = vTargetPos;
        m_pParticleSystemCom->Set_Parameter(tParticleParam);


        Compute_ViewZ(&vTargetPos);
        m_fViewZ += 10.f;
    }


    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectSmokeTrail::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    // render particle
    m_pTextureCom->Set_Texture(4); // 3, 4번이 자연스러운듯

    _matrix matWorld;
    D3DXMatrixIdentity(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pParticleSystemCom->Render_Parcitle();
}

CEffectSmokeTrail* CEffectSmokeTrail::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectSmokeTrail* pEffect = new CEffectSmokeTrail(_pGraphicDev);



    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("CEffectSmokeTrail create failed");
        return nullptr;
    }

    return pEffect;
}

void CEffectSmokeTrail::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CEffectSmokeTrail::Add_Component()
{
    Engine::CComponent* pComponent(nullptr);

    //pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_Glow_White"));
    pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_SmokeTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pBufferCom = static_cast<Engine::CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
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

    pComponent = m_pParticleSystemCom = static_cast<Engine::CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem", pComponent });

    return S_OK;
}

void CEffectSmokeTrail::Set_ParticleParameter()
{
    CParticleSystem::PARAM tParticleParam;
    ZeroMemory(&tParticleParam, sizeof(CParticleSystem::PARAM));

    tParticleParam.tInit.tHexahedron.vInitVelocity = { 0.f, 0.f, 0.f };
    tParticleParam.vVelocityNoise = { 0.3f, 0.3f, 0.3f };
    tParticleParam.vColor = _vec4(0.6f, .6f, .6f, 1.5f);
    tParticleParam.vColorFade = _vec4(0.2f, 0.2f, 0.2f, 0.0f);
    tParticleParam.iTotalCnt = 500;

    tParticleParam.fSize = 1.0f;
    tParticleParam.fLifeTime = 1.5f;

    tParticleParam.fEmitRate = 75.f;
    tParticleParam.iEmitCnt = 3;

    m_pParticleSystemCom->Set_Parameter(tParticleParam);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
    //m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::POINT_SCALE_DISABLE, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
    m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::ALPHA_SORT, TRUE);


    m_pParticleSystemCom->SetUp_Particle();

}

void CEffectSmokeTrail::OnOperate(void* _pParam)
{
    CEffectSmokeTrail* pThis = (CEffectSmokeTrail*)_pParam;

    pThis->m_pParticleSystemCom->Reset();

    pThis->m_bIsMissing = FALSE;
    pThis->m_fGraceTime = 0.f;
}
