#include "pch.h"
#include "../Header/EffectFlash.h"
#include "Export_Utility.h"

CEffectFlash::CEffectFlash(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectFlash::~CEffectFlash()
{
}

HRESULT CEffectFlash::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_fViewZ = 100.0f;
    m_pTransformCom->Set_Pos(0.f, 0.f, m_fViewZ);
    m_pTransformCom->Set_Scale(WINCX, WINCY, 1.f);

    m_pEffectCom->Set_LifeTime(0.5f);

    return S_OK;
}

_int CEffectFlash::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectFlash::LateUpdate_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectFlash::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;
    
    _float fValue = 1.f - m_pEffectCom->Get_ElapsedPersentage();
    _vec4 vFlashColor = m_vColor * fValue;
    
    DWORD dwPreCOLOROP, dwPreCONSTANT, dwPreCOLOCARG1, dwPreCOLORARG2;
    DWORD dwPreALPHAOP, dwPreALPHAARG1, dwPreALPHAARG2;

    m_pGraphicDev->GetTextureStageState(0, D3DTSS_CONSTANT, &dwPreCONSTANT);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwPreCOLOROP);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG1, &dwPreCOLOCARG1);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG2, &dwPreCOLORARG2);

    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwPreALPHAOP);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwPreALPHAARG1);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwPreALPHAARG2);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_COLORVALUE(vFlashColor.x, vFlashColor.y, vFlashColor.z, vFlashColor.w));
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CONSTANT);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CONSTANT);


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

CEffectFlash* CEffectFlash::Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec4& _vColor)
{
    CEffectFlash* pEffect = new CEffectFlash(_pGraphicDev);
    pEffect->m_vColor = _vColor;

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("CEffectFlash create failed");
        return nullptr;
    }

    return pEffect;
}

void CEffectFlash::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CEffectFlash::Add_Component()
{
    Engine::CComponent* pComponent(nullptr);
    pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_SquareTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pBufferCom = static_cast<Engine::CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

    return S_OK;
}
