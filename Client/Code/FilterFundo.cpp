#include "pch.h"
#include "../Header/FilterFundo.h"
#include "Export_Utility.h"

CFilterFundo::CFilterFundo(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CFilterFundo::~CFilterFundo()
{
}

CFilterFundo* CFilterFundo::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CFilterFundo* pFilter = new CFilterFundo(_pGraphicDev);

    if (FAILED(pFilter->Ready_GameObject(), E_FAIL))
    {
        Safe_Release(pFilter);
        MSG_BOX("Filter Fundo create Failed");
        return nullptr;
    }

    return pFilter;
}

HRESULT CFilterFundo::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(3.f, 2.f, 3.f);
    m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
    // 이 필터가 항상 무족권 알파 소팅되었을 때 첫번째이어야함
    // 이유는 모름 무적권 alpha 렌더 그룹중엔 무적권 첫번째로 나와야 다른것들도 보임
    m_fViewZ = 10000000000.0f;

    return S_OK;
}

_int CFilterFundo::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

    //CComponent* pComponenet = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform");
    //_vec3 vPos, vLook;
    //static_cast<CTransform*>(pComponenet)->Get_Info(INFO::INFO_POS, &vPos);
    //static_cast<CTransform*>(pComponenet)->Get_Info(INFO::INFO_LOOK, &vLook);
    ////vLook.y = 0.f;
    //vLook = { vLook.x, 0.f, vLook.z };
    //m_pTransformCom->Set_Pos(vPos + vLook * 0.2f + _vec3(0.f, 0.5f, 0.f));

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CFilterFundo::LateUpdate_GameObject()
{

    CComponent* pComponenet = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform");
    _vec3 vPos, vLook;
    static_cast<CTransform*>(pComponenet)->Get_Info(INFO::INFO_POS, &vPos);
    static_cast<CTransform*>(pComponenet)->Get_Info(INFO::INFO_LOOK, &vLook);
    //vLook.y = 0.f;
    vLook = { vLook.x, 0.f, vLook.z };
    m_pTransformCom->Set_Pos(vPos + vLook * 0.2f + _vec3(0.f, 0.5f, 0.f));
    m_pTransformCom->Update_Component(0.f); // 이러면 안된다는걸 알지만..

    // 빌보드
    const _vec3* pScale, * pAngle;
    //_vec3 vPos;
    pScale = m_pTransformCom->Get_Scale();
    pAngle = m_pTransformCom->Get_Angle();
    m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

    _matrix matScale, matRotationZ, matScaleInverse;
    D3DXMatrixScaling(&matScale, pScale->x, pScale->y, pScale->z);
    D3DXMatrixRotationZ(&matRotationZ, pAngle->z);
    D3DXMatrixInverse(&matScaleInverse, 0, &matScale);


    _vec3 vCamPos;
    _matrix matBillboardY;
    _matrix matView;
    D3DXMatrixIdentity(&matBillboardY);

    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

    matBillboardY._11 = matView._11;
    matBillboardY._13 = matView._13;
    matBillboardY._31 = matView._31;
    matBillboardY._33 = matView._33;

    D3DXMatrixInverse(&matBillboardY, 0, &matBillboardY);

    _matrix	matRot[(_uint)ROTATION::ROT_END];
    for (int i = 0; i < 3; ++i)
        D3DXMatrixIdentity(&matRot[i]);
    //D3DXMatrixRotationX(&matRot[(_uint)ROTATION::ROT_X], pAngle->x);
    //D3DXMatrixRotationY(&matRot[(_uint)ROTATION::ROT_Y], pAngle->y);
    D3DXMatrixRotationZ(&matRot[(_uint)ROTATION::ROT_Z], pAngle->z);
    _matrix matTotalRot, matTotalRotInverse;
    D3DXMatrixIdentity(&matTotalRot);
    matTotalRot = matRot[0] * matRot[1] * matRot[2];

    D3DXMatrixInverse(&matTotalRotInverse, 0, &matTotalRot);


    _matrix matWorld;
    m_pTransformCom->Get_WorldMatrix(&matWorld);
    //D3DXMatrixTranslation(&matWorld, vPos.x, vPos.y, vPos.z);

    matWorld = matScale * matTotalRot * matBillboardY * matTotalRotInverse * matScaleInverse * matWorld;

    m_pTransformCom->Set_WorldMatrix(&matWorld);



    Engine::CGameObject::LateUpdate_GameObject();
}

void CFilterFundo::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    //m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    DWORD dwPreCOLOROP, dwPreCONSTANT, dwPreCOLOCARG1, dwPreCOLORARG2;
    DWORD dwPreALPHAOP, dwPreALPHAARG1, dwPreALPHAARG2;
    DWORD dwPreTextureFactor;

    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwPreALPHAOP);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwPreALPHAARG1);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwPreALPHAARG2);

    m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &dwPreTextureFactor);


    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(96, 255, 255, 255));
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    m_pTextureCom->Set_Texture(2);
    m_pBufferCom->Render_Buffer();


    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, dwPreTextureFactor);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwPreALPHAOP);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwPreALPHAARG1);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, dwPreALPHAARG2);
}

HRESULT CFilterFundo::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    pComponent->SetOwner(*this);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FUNDOTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

void CFilterFundo::Free()
{
    Engine::CGameObject::Free();
}
