#include "pch.h"
#include "../Header/EffectMuzzleFlash.h"
#include "Export_Utility.h"

CEffectMuzzleFlash::CEffectMuzzleFlash(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
    , m_pTextureCom(nullptr)
    , m_pBufferCom(nullptr)
    , m_pEffectCom(nullptr)
    , m_iTotalFrame(0)
    , m_iCurFrame(0)
{
}

CEffectMuzzleFlash::~CEffectMuzzleFlash()
{
}

CEffectMuzzleFlash* CEffectMuzzleFlash::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectMuzzleFlash* pMuzzleFlash = new CEffectMuzzleFlash(_pGraphicDev);

    if (FAILED(pMuzzleFlash->Ready_GameObject(), E_FAIL))
    {
        Safe_Release(pMuzzleFlash);
        MSG_BOX("Effect MuzzleFlash create Failed");
        return nullptr;
    }

    return pMuzzleFlash;
}

HRESULT CEffectMuzzleFlash::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_fViewZ = 20.f;
    m_vInitPosition = { 40.f, 40.f, m_fViewZ };
    for (_uint i = 0; i < m_vecTransformCom.size(); ++i)
    {
        m_vecTransformCom[i]->Set_Pos(m_vInitPosition);
        m_vecTransformCom[i]->Set_Scale(100.f, 100.f, 1.f);
    }
    m_pEffectCom->Set_LifeTime(0.3f);
    m_pEffectCom->Set_CallBack(Set_RandomRotation);
    m_pEffectCom->Set_CallBackParam(this);
    m_iTotalFrame = 1 * 7;

    D3DLIGHT9		tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

    tLightInfo.Type = D3DLIGHT_POINT;

    tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f };
    tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f };
    tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f };
    // temp code // player Position
    Engine::_vec3 vPos = { 0.f, 0.f, 0.f, };
    tLightInfo.Position = vPos;
    tLightInfo.Range = 10.f;

    FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);

    return S_OK;
}

_int CEffectMuzzleFlash::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);


    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectMuzzleFlash::LateUpdate_GameObject()
{
    _float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
    if (m_pEffectCom->Get_Visibility())
    {
        if (fPersentage < 0.2f)
        {
            m_pGraphicDev->LightEnable(1, TRUE);
        }
        else
        {
            m_pGraphicDev->LightEnable(1, FALSE);
        }
    }
    m_iCurFrame = m_iTotalFrame * fPersentage;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectMuzzleFlash::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    //m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pTextureCom->Set_Texture();

    //Setup_Material();
    Animate_UV();
    for (_uint i = 0; i < m_vecTransformCom.size(); ++i)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_vecTransformCom[i]->Get_WorldMatrix());
        m_pBufferCom->Render_Buffer();
    }

    //m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

}

HRESULT CEffectMuzzleFlash::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MuzzleFlashTexture"));
    //pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    constexpr _uint iSize = 5;
    _tchar szTransformTag[iSize][32];
    for (_uint i = 0; i < iSize; ++i)
    {
        m_vecTransformCom.push_back(static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform")));
        pComponent = m_vecTransformCom.back();
        NULL_CHECK_RETURN(pComponent, E_FAIL);
        pComponent->SetOwner(*this);
        swprintf_s(szTransformTag[i], 32, L"Com_Transform%d", i);
        m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ szTransformTag[i], pComponent });
    }

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

    return S_OK;
}

HRESULT CEffectMuzzleFlash::Setup_Material()
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

void CEffectMuzzleFlash::Free()
{
    Engine::CGameObject::Free();
}

void CEffectMuzzleFlash::Animate_UV()
{
    _float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
    m_iCurFrame = m_iTotalFrame * fPersentage;

    m_pBufferCom->Set_UV(_vec2(7.f, 1.f), m_iCurFrame);
}

void CEffectMuzzleFlash::Reset()
{
    for (_uint i = 0; i < m_vecTransformCom.size(); ++i)
    {
        m_vecTransformCom[i]->Set_Pos(m_vInitPosition);
    }
    //m_pEffectCom->Set_Visibility(FALSE);
    m_pEffectCom->Stop_Effect();
}

void CEffectMuzzleFlash::Set_RandomRotation(void* _this)
{
    CEffectMuzzleFlash* pThis = reinterpret_cast<CEffectMuzzleFlash*>(_this);

    CComponent* pComponent(nullptr);


    pThis->Reset();

    _float fRadius = 0.75f;
    _vec3 vRight, vUp, vPos;
    _matrix matRotation;

    for (_uint i = 0; i < pThis->m_vecTransformCom.size(); ++i)
    {
        _float fRandom = rand() % 1000 * 0.001f;
        fRandom *= 360.f;
        D3DXMatrixIdentity(&matRotation);
        D3DXMatrixRotationZ(&matRotation, fRandom);
        pThis->m_vecTransformCom[i]->Get_Info(INFO::INFO_RIGHT, &vRight);
        //pThis->m_vecTransformCom[i]->Get_Info(INFO::INFO_RIGHT, &vUp);
        pThis->m_vecTransformCom[i]->Get_Info(INFO::INFO_POS, &vPos);

        D3DXVec3TransformNormal(&vRight, &vRight, &matRotation);
        //D3DXVec3TransformNormal(&vUp, &vUp, &matRotation);

        pThis->m_vecTransformCom[i]->Set_Pos(vPos - vRight * fRadius);
        pThis->m_vecTransformCom[i]->Rotation(ROTATION::ROT_Z, fRandom);

    }

    D3DLIGHT9 tLight;
    pThis->m_pGraphicDev->GetLight(1, &tLight);
    pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform");
    dynamic_cast<CTransform*>(pComponent)->Get_Info(INFO::INFO_POS, &vPos);
    tLight.Position = vPos;
    pThis->m_pGraphicDev->SetLight(1, &tLight);
}
