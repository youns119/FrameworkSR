#include "pch.h"
#include "../Header/EffectMinigunShell.h"
#include "Export_Utility.h"

CEffectMinigunShell::CEffectMinigunShell(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectMinigunShell::~CEffectMinigunShell()
{
}

HRESULT CEffectMinigunShell::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_fViewZ = 21.f;

    for (_uint i = 0; i < OBJ_END; ++i)
    {
		m_pTransformCom[i]->Set_Pos(0.f, 0.f, m_fViewZ);
		m_pTransformCom[i]->Set_Scale(50.f, 50.f, 1.f);
    }

    m_pEffectCom->Set_LifeTime(0.5f);
    m_pEffectCom->Set_CallBack(OnOperate);
    m_pEffectCom->Set_CallBackParam(this);

    m_iTotalFrame[BULLET] = 4 * 2;
    m_iTotalFrame[CAPSULE] = 3 * 1;

    m_iRandomSpriteOffset = rand() % 10000 * 0.0001 * 3;


    return S_OK;
}

_int CEffectMinigunShell::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

    _vec3 vPosition;
    for (_uint i = 0; i < OBJ_END; ++i)
    {
		m_pTransformCom[i]->Get_Info(INFO::INFO_POS, &vPosition);
		m_vVelocity[i] += _vec3(0.0f, -9.8f * 150.f, 0.f) * _fTimeDelta;
		vPosition += m_vVelocity[i] * _fTimeDelta;
		m_pTransformCom[i]->Set_Pos(vPosition);
    }

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectMinigunShell::LateUpdate_GameObject()
{
    Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectMinigunShell::Render_GameObject()
{
    if (!m_pEffectCom->Get_Visibility())
        return;

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    _float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
    m_iCurFrame[BULLET] = m_iTotalFrame[BULLET] * fPersentage + m_iRandomSpriteOffset;
    m_iCurFrame[CAPSULE] = m_iTotalFrame[CAPSULE] * fPersentage * 2.f + m_iRandomSpriteOffset;

    m_iCurFrame[CAPSULE] %= m_iTotalFrame[CAPSULE];
    m_iCurFrame[BULLET] %= m_iTotalFrame[BULLET];


    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[CAPSULE]->Get_WorldMatrix());
    m_pBufferCom->Set_UV(_vec2(3.f, 1.f), m_iCurFrame[CAPSULE]);

    m_pTextureCom[CAPSULE]->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[BULLET]->Get_WorldMatrix());
    m_pBufferCom->Set_UV(_vec2(4.f, 2.f), m_iCurFrame[BULLET]);

    m_pTextureCom[BULLET]->Set_Texture();
    m_pBufferCom->Render_Buffer();

}

CEffectMinigunShell* CEffectMinigunShell::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CEffectMinigunShell* pEffect = new CEffectMinigunShell(_pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("Effect Minigun Shell create failed");
        return nullptr;
    }

    return pEffect;
}

void CEffectMinigunShell::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CEffectMinigunShell::Add_Component()
{
    Engine::CComponent* pComponent(nullptr);

    pComponent = m_pTextureCom[BULLET] = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_MinigunShellTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_TextureBullet", pComponent });

    pComponent = m_pTextureCom[CAPSULE] = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_MinigunCapsuleTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_TextureCapsule", pComponent });

    pComponent = m_pBufferCom = static_cast<Engine::CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom[CAPSULE] = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_TransformCapsule", pComponent });

    pComponent = m_pTransformCom[BULLET] = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_TransformBullet", pComponent });

    pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

    return S_OK;
}

void CEffectMinigunShell::OnOperate(void* _pParam)
{
    CEffectMinigunShell* pThis = (CEffectMinigunShell*)_pParam;

    CTransform* pTransformComCapsule = static_cast<CTransform*>(pThis->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_TransformCapsule"));
    CTransform* pTransformComBullet = static_cast<CTransform*>(pThis->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_TransformBullet"));

    for (_uint i = 0; i < OBJ_END; ++i)
    {
		pThis->m_vVelocity[i] = {-600.f, 1200.f, 0.f};
		pThis->m_vVelocity[i].x += (rand() % 1000 * 0.001f) * 250.f - 500.f;
		pThis->m_vVelocity[i].y += (rand() % 1000 * 0.001f) * 250.f - 500.f;

    }

    pTransformComCapsule->Set_Pos(0.f, 0.f, pThis->m_fViewZ);
    pTransformComBullet->Set_Pos(0.f, 0.f, pThis->m_fViewZ);
}
