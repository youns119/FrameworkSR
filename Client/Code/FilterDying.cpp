#include "pch.h"
#include "../Header/FilterDying.h"
#include "Export_Utility.h"
#include "../Header/Player.h"

CFilterDying::CFilterDying(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
}

CFilterDying::~CFilterDying()
{
}

CFilterDying* CFilterDying::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CFilterDying* pFilter = new CFilterDying(_pGraphicDev);

    if (FAILED(pFilter->Ready_GameObject(), E_FAIL))
    {
        Safe_Release(pFilter);
        MSG_BOX("Filter Dying create Failed");
        return nullptr;
    }

    return pFilter;
}

HRESULT CFilterDying::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_fViewZ = 0.1f;
    m_pTransformCom->Set_Pos(0.f, 0.f, m_fViewZ);
    m_pTransformCom->Set_Scale(WINCX, WINCY, 1.f);

    return S_OK;
}

_int CFilterDying::Update_GameObject(const _float& _fTimeDelta)
{
    Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

    //static float fAlpha = 0.f;

    //ImGui::Begin("dying");

    //ImGui::DragFloat("Alpha", &fAlpha, 0.05f, 0.f, 1.f);

    //ImGui::End();

    //m_fAlphaRef = fAlpha;

    CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));
    _float fTimerHP = pPlayer->Get_TimerHP();
    _float fPlayerHP = pPlayer->Get_PlayerHP();

    if (pPlayer->Get_BossStage())
    {
        if (fPlayerHP <= 20.f)
        {
            m_bIsRender = TRUE;
            m_fAlphaRef = 1.f - fPlayerHP / 20.f;
            m_fAlphaRef = max(0.f, m_fAlphaRef);
            m_fAlphaRef = min(1.f, m_fAlphaRef);
        }
        else
            m_bIsRender = FALSE;
    }
    else
    {
		if (fTimerHP <= 4.f)
		{
			m_bIsRender = TRUE;
			m_fAlphaRef = 1.f - fTimerHP / 4.f;
			m_fAlphaRef = max(0.f, m_fAlphaRef);
			m_fAlphaRef = min(1.f, m_fAlphaRef);
		}
		else
		{
			m_bIsRender = FALSE;
		}
    }

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CFilterDying::LateUpdate_GameObject()
{
    Engine::CGameObject::LateUpdate_GameObject();
}

void CFilterDying::Render_GameObject()
{
    if (!m_bIsRender)
        return;

    DWORD dwPreCOLOROP, dwPreCONSTANT, dwPreCOLOCARG1, dwPreCOLORARG2;
    DWORD dwPreALPHAOP, dwPreALPHAARG1, dwPreALPHAARG2;

    m_pGraphicDev->GetTextureStageState(0, D3DTSS_CONSTANT, &dwPreCONSTANT);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwPreCOLOROP);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG1, &dwPreCOLOCARG1);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG2, &dwPreCOLORARG2);

    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwPreALPHAOP);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwPreALPHAARG1);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwPreALPHAARG2);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_COLORVALUE(0.f, 0.f, 0.f, m_fAlphaRef));
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

HRESULT CFilterDying::Add_Component()
{
	Engine::CComponent* pComponent(nullptr);
	pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_Texure_Dying"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pBufferCom = static_cast<Engine::CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	//pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//pComponent->SetOwner(*this);
	//m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

	return S_OK;
}

void CFilterDying::Free()
{
    Engine::CGameObject::Free();
}
