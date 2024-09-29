#include "pch.h"
#include "../Header/EffectBossRobotBooster.h"
#include "Export_Utility.h"

CEffectBossRobotBooster::CEffectBossRobotBooster(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
    , m_pTarget(nullptr)
{
}

CEffectBossRobotBooster::~CEffectBossRobotBooster()
{
}

CEffectBossRobotBooster* CEffectBossRobotBooster::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectBossRobotBooster* pEffect = new CEffectBossRobotBooster(_pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject(), E_FAIL))
    {
        Safe_Release(pEffect);
        MSG_BOX("Effect BossRobotBooster create Failed");
        return nullptr;
    }

    return pEffect;
}

HRESULT CEffectBossRobotBooster::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(3.f, 2.f, 3.f);
    m_pTransformCom->Set_Scale(2.f, 1.f, 1.f);
    m_pEffectCom->Set_LifeTime(0.3f);
    m_pEffectCom->Set_Repeatable(TRUE);
    m_iTotalFrame[FLARE] = 7;
    m_iTotalFrame[TANK] = 8;

    return S_OK;
}

_int CEffectBossRobotBooster::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

    CComponent* pComponenet = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform");
    _vec3 vPos, vLook; 
    static_cast<CTransform*>(pComponenet)->Get_Info(INFO::INFO_POS, &vPos);
    static_cast<CTransform*>(pComponenet)->Get_Info(INFO::INFO_LOOK, &vLook);
    m_pTransformCom->Set_Pos(vPos + vLook * 5.f);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectBossRobotBooster::LateUpdate_GameObject()
{
    _float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
    m_iCurFrame[FLARE] = m_iTotalFrame[FLARE] * fPersentage;
    m_iCurFrame[TANK] = m_iTotalFrame[TANK] * fPersentage;

    Engine::CGameObject::LateUpdate_GameObject();

}

void CEffectBossRobotBooster::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_float fPersentage = m_pEffectCom->Get_ElapsedPersentage();

    for (_uint i = 0; i < IDX_END; ++i)
    {
		m_iCurFrame[i] = m_iTotalFrame[i] * fPersentage;
		m_pTextureCom[i]->Set_Texture();

		m_pBufferCom->Set_UV(_vec2(m_iTotalFrame[i], 1.f), m_iCurFrame[i] + (i * 4)); // ㅋㅋ 인덱스 오프셋주기

		m_pBufferCom->Render_Buffer();
    }

}

HRESULT CEffectBossRobotBooster::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom[TANK] = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MuzzleTankTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_TANK", pComponent });

    pComponent = m_pTextureCom[FLARE] = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MuzzleFlashTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_FLARE", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

    return S_OK;
}

void CEffectBossRobotBooster::Free()
{
    Engine::CGameObject::Free();
}
