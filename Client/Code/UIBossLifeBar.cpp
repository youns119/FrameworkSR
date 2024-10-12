#include "pch.h"
#include "..\Header\UIBossLifeBar.h"
#include "..\Header\Boss_Robot.h"
#include "Export_Utility.h"

CUIBossLifeBar::CUIBossLifeBar(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformTexture(nullptr)
	, m_pTexture(nullptr)
	, m_pTextureSurface(nullptr)
	, m_pPreSurface(nullptr)
	, m_iCount(0)
	, m_fMaxHP(0.f)
	, m_fCurrHP(0.f)
	, m_fHitTime(0.f)
	, m_fAlpha(0.f)
{
	for (_uint i = 0; i < (_uint)UI_LIFEBAR::LIFEBAR_END - 1; ++i)
		m_pTextureCom[i] = nullptr;

	for (_uint i = 0; i < (_uint)UI_LIFEBAR::LIFEBAR_END; ++i)
		m_pTransformCom[i] = nullptr;

	for (_uint i = 0; i < 18; i++)
	{
		m_pBarTextureCom[i] = nullptr;
		m_pBarTransformCom[i] = nullptr;
	}
}

CUIBossLifeBar::~CUIBossLifeBar()
{
}

CUIBossLifeBar* CUIBossLifeBar::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIBossLifeBar* pUIBossLifeBar = new CUIBossLifeBar(_pGraphicDev);

	if (FAILED(pUIBossLifeBar->Ready_Unit()))
	{
		Safe_Release(pUIBossLifeBar);
		MSG_BOX("UIBossLifeBar Create Failed");
		return nullptr;
	}

	return pUIBossLifeBar;
}

HRESULT CUIBossLifeBar::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fAlpha = 1.f;

	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_P]->Set_Pos(0.f, 160.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_B]->Set_Pos(0.f, 160.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR]->Set_Pos(0.f, 160.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_A]->Set_Pos(0.f, 160.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LOGO]->Set_Pos(-127.f, 160.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_BATTERY]->Set_Pos(0.f, 195.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_TEXT]->Set_Pos(0.f, 135.f, 0.f);
	m_pTransformTexture->Set_Pos(0.f, 0.f, 0.f);

	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_P]->Set_Scale(160.f, 160.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_B]->Set_Scale(160.f, 160.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR]->Set_Scale(160.f, 160.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_A]->Set_Scale(160.f, 160.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LOGO]->Set_Scale(20.f, 20.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_BATTERY]->Set_Scale(135.f, 135.f, 0.f);
	m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_TEXT]->Set_Scale(110.f, 110.f, 0.f);
	m_pTransformTexture->Set_Scale(WINCX / 2.f, WINCY / 2.f, 0.f);

	_int iOffset = 0;

	for (_int i = 0; i < 18; i++)
	{
		if (i >= 6 && i < 12)
			iOffset = 1;
		else if (i >= 12)
			iOffset = 2;

		m_pBarTransformCom[i]->Set_Pos(-97.f + 7.f * iOffset + 12.5f * i, 160.f, 0.f);

		m_pBarTransformCom[i]->Set_Scale(7.f, 23.f, 0.f);
	}

	m_pGraphicDev->CreateTexture
	(
		WINCX, WINCY,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexture,
		NULL
	);

	m_pTexture->GetSurfaceLevel(0, &m_pTextureSurface);
	m_pGraphicDev->GetRenderTarget(0, &m_pPreSurface);

	m_bRender = true;

	return S_OK;
}

_int CUIBossLifeBar::Update_Unit(const _float& _fTimeDelta)
{
	CBoss_Robot* pBoss = static_cast<CBoss_Robot*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Monster", L"Boss_Robot"));

	m_fMaxHP = pBoss->Get_BossMaxHP();
	m_fCurrHP = pBoss->Get_BossHP();

	_float fStep = m_fMaxHP / 18.f;
	m_iCount = m_fCurrHP / fStep + 1;
	if (m_iCount > 18) m_iCount = 18;

	if (m_bHit)
		if (m_fHitTime < 1.f)
		{
			m_fHitTime += _fTimeDelta;

			if (m_fHitTime >= 1.f)
			{
				m_fHitTime = 0.f;
				m_bHit = false;
			}
		}

	if (m_bHit)
	{
		if (m_fAlpha > 0.f)
		{
			m_fAlpha -= _fTimeDelta;
			if (m_fAlpha <= 0.f) m_fAlpha = 0.f;
		}
	}

	Hit_Effect();

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIBossLifeBar::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIBossLifeBar::Render_Unit()
{
	if (m_bHit)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_P]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_P]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_B]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_B]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}

	{
		m_pGraphicDev->GetRenderTarget(0, &m_pPreSurface);
		m_pGraphicDev->SetRenderTarget(0, m_pTextureSurface);
		m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		if (m_bHit)
		{
			DWORD tPreFactor;
			m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_A]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_A]->Set_Texture();
			m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_A]->Change_Alpha(m_fAlpha);
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LOGO]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LOGO]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		for (_int i = 0; i < m_iCount; i++)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBarTransformCom[i]->Get_WorldMatrix());

			m_pBarTextureCom[i]->Set_Texture();
			m_pBufferCom->Render_Buffer();
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_BATTERY]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_BATTERY]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_TEXT]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_TEXT]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		{
			m_pGraphicDev->SetRenderTarget(0, m_pPreSurface);

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformTexture->Get_WorldMatrix());

			m_pGraphicDev->SetTexture(0, m_pTexture);
			m_pBufferCom->Render_Buffer();
		}
	}
}

HRESULT CUIBossLifeBar::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	{
		pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_P] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_LifeBar_P"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_LifeBar_P", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_B] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_LifeBar_B"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_LifeBar_B", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_LifeBar"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_LifeBar", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_A] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_LifeBar_A"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_LifeBar_A", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_LOGO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_Logo"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Logo", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_BATTERY] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_Battery"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Battery", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_LIFEBAR::LIFEBAR_TEXT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Roboto"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Text", pComponent });
	}

	for (_uint i = 0; i < 18; i++)
	{
		pComponent = m_pBarTextureCom[i] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIBoss_Bar"));
		NULL_CHECK_RETURN(m_pBarTextureCom[i], E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Bar", m_pBarTextureCom[i] });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_P] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_LifeBar_P", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_B] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_LifeBar_B", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_LifeBar", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_A] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_LifeBar_A", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LOGO] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Logo", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_BATTERY] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Battery", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_TEXT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Text", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_BAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Bar", pComponent });

		pComponent = m_pTransformTexture = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Texture", pComponent });
	}

	for (_uint i = 0; i < 18; i++)
	{
		pComponent = m_pBarTransformCom[i] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Bar", pComponent });
	}

	return S_OK;
}

void CUIBossLifeBar::Hit_Effect()
{
	if (!m_bHit)
	{
		m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_P]->Set_Pos(0.f, 160.f, 0.f);
		m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_B]->Set_Pos(0.f, 160.f, 0.f);
		m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR]->Set_Pos(0.f, 160.f, 0.f);
		m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_A]->Set_Pos(0.f, 160.f, 0.f);
	}
	else
	{
		_float fGap = 1.f - m_fHitTime;
		if ((int)(fGap * 15.f) <= 0) return;

		_int iRandX = (int)(fGap * 7.f) - rand() % (int)(fGap * 15.f);
		_int iRandY = (int)(fGap * 7.f) - rand() % (int)(fGap * 15.f);
		m_pTransformTexture->Set_Pos(0.f + iRandX, 0.f + iRandY, 0.f);

		iRandX = (int)(fGap * 7.f) - rand() % (int)(fGap * 15.f);
		iRandY = (int)(fGap * 7.f) - rand() % (int)(fGap * 15.f);
		m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_P]->Set_Pos(0.f + iRandX, 160.f + iRandY, 0.f);

		iRandX = (int)(fGap * 7.f) - rand() % (int)(fGap * 15.f);
		iRandY = (int)(fGap * 7.f) - rand() % (int)(fGap * 15.f);
		m_pTransformCom[(_uint)UI_LIFEBAR::LIFEBAR_LIFEBAR_B]->Set_Pos(0.f + iRandX, 160.f + iRandY, 0.f);
	}
}

void CUIBossLifeBar::Reset()
{
	m_iCount = 0;
	m_fAlpha = 1.f;
}

void CUIBossLifeBar::Free()
{
	m_pTextureSurface->Release();
	m_pPreSurface->Release();
	m_pTexture->Release();

	Engine::CUIUnit::Free();
}