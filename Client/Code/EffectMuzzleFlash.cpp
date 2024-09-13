#include "pch.h"
#include "../Header/EffectMuzzleFlash.h"
#include "Export_Utility.h"

CEffectMuzzleFlash::CEffectMuzzleFlash(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
    , m_pTransformCom(nullptr)
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
    m_pTransformCom->Set_Pos(15.f, 2.f, 10.f);
    m_pEffectCom->Set_LifeTime(0.5f);
    m_iTotalFrame = 16;

    D3DLIGHT9		tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

    tLightInfo.Type = D3DLIGHT_POINT;

    tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f };
    tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f };
    tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f };
    Engine::_vec3 vPos; m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
    tLightInfo.Position = { 15.f, 2.f, 9.f };
    tLightInfo.Range = 10.f;

    FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1), E_FAIL);

    return S_OK;
}

_int CEffectMuzzleFlash::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);


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

    _vec3	vTemp;
    m_pTransformCom->Get_Info(Engine::INFO::INFO_POS, &vTemp);
    CGameObject::Compute_ViewZ(&vTemp);

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectMuzzleFlash::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pTextureCom->Set_Texture(m_iCurFrame);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

HRESULT CEffectMuzzleFlash::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MuzzleFlashTexture"));
    //pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
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
