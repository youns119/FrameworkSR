#include "pch.h"
#include "../Header/EffectFanSpread.h"
#include "Export_Utility.h"

CEffectFanSpread::CEffectFanSpread(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectFanSpread::~CEffectFanSpread()
{
}

HRESULT CEffectFanSpread::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(3.f, 1.f, 3.f);
    m_pEffectCom->Set_LifeTime(0.5f);
    //m_iTotalFrame = 4 * 3;
    m_iTotalFrame = 8 * 4;

    return S_OK;
}

_int CEffectFanSpread::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectFanSpread::LateUpdate_GameObject()
{
    _vec3	vTemp;
    m_pTransformCom->Get_Info(Engine::INFO::INFO_POS, &vTemp);
    CGameObject::Compute_ViewZ(&vTemp);

    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectFanSpread::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pTextureCom->Set_Texture();

    Animate_UV();
    m_pBufferCom->Render_Buffer();
}

CEffectFanSpread* CEffectFanSpread::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectFanSpread* pEffect = new CEffectFanSpread(_pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("Effect Blood Splater create failed");
        return nullptr;
    }

    return pEffect;
}

void CEffectFanSpread::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CEffectFanSpread::Add_Component()
{
    Engine::CComponent* pComponent(nullptr);

    pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_FanSpreadTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pBufferCom = static_cast<Engine::CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });
}

void CEffectFanSpread::Animate_UV()
{
	_float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
	m_iCurFrame = m_iTotalFrame * fPersentage;

	//m_pBufferCom->Set_UV(_vec2(4.f, 3.f), m_iCurFrame);
	m_pBufferCom->Set_UV(_vec2(8.f, 4.f), m_iCurFrame);
}
