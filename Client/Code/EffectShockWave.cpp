#include "pch.h"
#include "../Header/EffectShockWave.h"
#include "Export_Utility.h"

CEffectShockWave::CEffectShockWave(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectShockWave::~CEffectShockWave()
{
}

HRESULT CEffectShockWave::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(3.f, 1.f, 3.f);
    m_vMaxSize = { 7.f, 7.f, 7.f };
    m_vMinSize = { 0.1f, 0.1f, 0.1f };

    m_vInitColor = { 255.f / 255.f, 180.f / 255.f, 0.f, 1.f };
    m_vFadeColor = { 255.f / 255.f, 180.f / 255.f, 0.f, 0.f };

    m_pEffectCom->Set_LifeTime(0.25f);
    m_pEffectCom->Set_Repeatable(FALSE);
    m_pEffectCom->Set_Visibility(TRUE);
    m_pEffectCom->Set_Billboard(TRUE);
    m_pEffectCom->Set_CallBack(OnOperate);
    m_pEffectCom->Set_CallBackParam(this);

    return S_OK;
}

_int CEffectShockWave::Update_GameObject(const _float& _fTimeDelta)
{
    if (!m_pEffectCom->Get_Visibility())
        return 0;

    Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

    _vec3 vCurSize;
    D3DXVec3Lerp(&vCurSize, &m_vMinSize, &m_vMaxSize, m_pEffectCom->Get_ElapsedPersentage());
    m_pTransformCom->Set_Scale(vCurSize);

    //_vec3 vPos, vLook;
    //static_cast<CTransform*>(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform"))
    //    ->Get_Info(INFO::INFO_POS, &vPos);
    //static_cast<CTransform*>(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform"))
    //    ->Get_Info(INFO::INFO_LOOK, &vLook);
    //m_pTransformCom->Set_Pos(vPos + vLook * 2.f + _vec3(0.f, 0.5f, 0.f));


    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectShockWave::LateUpdate_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectShockWave::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    D3DCOLOR dwCurColor;
    _vec4 vCurColor;
    D3DXVec4Lerp(&vCurColor, &m_vInitColor, &m_vFadeColor, m_pEffectCom->Get_ElapsedPersentage());
    vCurColor.x = vCurColor.x < 0.f ? 0.f : vCurColor.x;
    vCurColor.y = vCurColor.y < 0.f ? 0.f : vCurColor.y;
    vCurColor.z = vCurColor.z < 0.f ? 0.f : vCurColor.z;
    vCurColor.w = vCurColor.w < 0.f ? 0.f : vCurColor.w;

    vCurColor.x = vCurColor.x > 1.f ? 1.f : vCurColor.x;
    vCurColor.y = vCurColor.y > 1.f ? 1.f : vCurColor.y;
    vCurColor.z = vCurColor.z > 1.f ? 1.f : vCurColor.z;
    vCurColor.w = vCurColor.w > 1.f ? 1.f : vCurColor.w;

    dwCurColor = D3DCOLOR_COLORVALUE(vCurColor.x, vCurColor.y, vCurColor.z, vCurColor.w);

    DWORD dwPreCOLOROP, dwPreCONSTANT, dwPreCOLOCARG1, dwPreCOLORARG2;
    DWORD dwPreALPHAOP, dwPreALPHAARG1, dwPreALPHAARG2;

    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwPreCOLOROP);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_CONSTANT, &dwPreCONSTANT);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG1, &dwPreCOLOCARG1);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG2, &dwPreCOLORARG2);

    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwPreALPHAOP);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwPreALPHAARG1);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwPreALPHAARG2);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_CONSTANT, dwCurColor);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CONSTANT);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CONSTANT);

    //m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();


    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, dwPreCOLOROP);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_CONSTANT, dwPreCONSTANT);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, dwPreCOLOCARG1);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, dwPreCOLORARG2);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwPreALPHAOP);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwPreALPHAARG1);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, dwPreALPHAARG2);
}

CEffectShockWave* CEffectShockWave::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectShockWave* pEffect = new CEffectShockWave(_pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("Shock wave Create failed");
        return nullptr;
    }

    return pEffect;
}

void CEffectShockWave::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CEffectShockWave::Add_Component()
{
    CComponent* pComponent(nullptr);

    pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_Texure_FullCircle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

    return S_OK;
}

void CEffectShockWave::OnOperate(void* _pParam)
{
    CEffectShockWave* pThis = (CEffectShockWave*)_pParam;


    return;
}
