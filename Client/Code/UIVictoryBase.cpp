#include "pch.h"
#include "..\Header\UIVictoryBase.h"
#include "..\Header\UIScreen.h"
#include "..\Header\UIShop.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIVictoryBase::CUIVictoryBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureTransformCom(nullptr)
	, m_bTextureRendered(false)
	, m_bText(false)
	, m_fSpeed(0.f)
	, m_fDelayTime(0.f)
	, m_fFloorTime(0.f)
	, m_fWordTime(0.f)
	, m_iFloor(0)
	, m_iWordCount(0)
	, m_pTexture(nullptr)
	, m_pTextureSurface(nullptr)
	, m_pPreSurface(nullptr)
	, m_vTextPos{}
{
	for (_uint i = 0; i < (_uint)UI_VICTORY::VICTORY_END; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}

	for (_int i = 0; i < 3; ++i)
		m_fScaleRatio[i] = 0.f;

	for (_int i = 0; i < 5; ++i)
		m_iDigit[i] = 0;

	for (_int i = 0; i < 2; ++i)
		m_fAlpha[i] = 0.f;
}

CUIVictoryBase::~CUIVictoryBase()
{
}

CUIVictoryBase* CUIVictoryBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIVictoryBase* pUIScreenVictory = new CUIVictoryBase(_pGraphicDev);

	if (FAILED(pUIScreenVictory->Ready_Unit()))
	{
		Safe_Release(pUIScreenVictory);
		MSG_BOX("UIScreenVictory create Failed");
		return nullptr;
	}

	return pUIScreenVictory;
}

HRESULT CUIVictoryBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	for (_int i = 0; i < 3; ++i)
		m_fScaleRatio[i] = 5.f;

	m_fAlpha[0] = 0.f;
	m_fAlpha[1] = 1.f;

	m_fSpeed = 10.f;

	m_vTextPos = { 430.f, 40.f };

	lstrcpy(m_szComment1, L"또 만나요! <3");
	lstrcpy(m_szComment2, L"하하하 감사합니다!");
	lstrcpy(m_szComment3, L"본 때를 보여주세요!");

	{
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_ICON]->Set_Pos(-WINCX / 2.f + 50.f, WINCY / 2.f - 50.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_LIVE]->Set_Pos(-WINCX / 2.f + 190.f, WINCY / 2.f - 50.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BALLOON]->Set_Pos(160.f, WINCY / 2.f - 100.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND]->Set_Pos(0.f, WINCY / 2.f - 150.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_VICTORY]->Set_Pos(0.f, WINCY / 2.f - 150.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_OMEDETO]->Set_Pos(0.f, WINCY / 2.f - 200.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Set_Pos(0.f, -WINCY / 2.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BAR]->Set_Pos(0.f, -WINCY / 2.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_FLOOR]->Set_Pos(0.f, 70.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DOT]->Set_Pos(0.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT1]->Set_Pos(-150.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT2]->Set_Pos(-100.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT3]->Set_Pos(-25.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT4]->Set_Pos(25.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT5]->Set_Pos(100.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_SEC]->Set_Pos(170.f, -17.f, 0.f);
		m_pTextureTransformCom->Set_Pos(0.f, 0.f, 0.f);
	}

	{
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_ICON]->Set_Scale(25.f, 25.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_LIVE]->Set_Scale(110.f, 110.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BALLOON]->Set_Scale(500.f, 500.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND]->Set_Scale(300.f, 300.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_VICTORY]->Set_Scale(100.f, 100.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_OMEDETO]->Set_Scale(400.f, 400.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Set_Scale(800.f, 500.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BAR]->Set_Scale(WINCX, WINCY / 11.f - 10.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_FLOOR]->Set_Scale(200.f, 200.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DOT]->Set_Scale(80.f, 80.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT1]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT2]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT3]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT4]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT5]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_SEC]->Set_Scale(40.f, 40.f, 0.f);
		m_pTextureTransformCom->Set_Scale(WINCX / 2.f, WINCY / 2.f, 0.f);
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

_int CUIVictoryBase::Update_Unit(const _float& _fTimeDelta)
{
	_vec3 vDir = { 0.f, 1.f, 0.f };
	_vec3 vPos;

	if (vPos.y < 0.f)
	{
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Move_Pos(&vDir, _fTimeDelta * 200.f, m_fSpeed);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BAR]->Move_Pos(&vDir, _fTimeDelta * 200.f, m_fSpeed);

		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Get_Info(INFO::INFO_POS, &vPos);

		if (vPos.y >= 0.f)
		{
			m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Set_Pos(0.f, 0.f, 0.f);
			m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BAR]->Set_Pos(0.f, 0.f, 0.f);
		}
	}

	if (m_fScaleRatio[0] > 1.f)
	{
		m_fScaleRatio[0] -= _fTimeDelta * 15.f;
		if (m_fScaleRatio[0] <= 1.f) m_fScaleRatio[0] = 1.f;

		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_VICTORY]->Set_Scale(100.f * m_fScaleRatio[0], 100.f * m_fScaleRatio[0], 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_FLOOR]->Set_Scale(200.f * m_fScaleRatio[0], 200.f * m_fScaleRatio[0], 0.f);
	}

	m_fDelayTime += _fTimeDelta;

	if (m_fDelayTime > 0.5f)
	{
		if (m_fScaleRatio[1] > 1.f)
		{
			m_fScaleRatio[1] -= _fTimeDelta * 15.f;
			if (m_fScaleRatio[1] <= 1.f) m_fScaleRatio[1] = 1.f;

			m_pTransformCom[(_uint)UI_VICTORY::VICTORY_OMEDETO]->Set_Scale(400.f * m_fScaleRatio[1], 400.f * m_fScaleRatio[1], 0.f);
		}
	}

	if (m_fDelayTime >= 1.f)
	{
		if (m_fScaleRatio[2] > 1.f)
		{
			m_fScaleRatio[2] -= _fTimeDelta * 15.f;
			if (m_fScaleRatio[2] <= 1.f) m_fScaleRatio[2] = 1.f;

			m_pTextureTransformCom->Set_Scale(WINCX / 2.f * m_fScaleRatio[2], WINCY / 2.f * m_fScaleRatio[2], 0.f);
		}
	}

	if (m_fDelayTime >= 2.5f)
		if (m_fAlpha[1] > 0.f)
		{
			m_fAlpha[1] -= _fTimeDelta * 1.5f;
			if (m_fAlpha[1] <= 0.f) m_fAlpha[1] = 0.f;
		}

	m_fAlpha[0] += _fTimeDelta * 0.8f;
	if (m_fAlpha[0] >= 1.f) m_fAlpha[0] = 1.f;

	m_iFloor = static_cast<CUIScreen*>(Engine::Get_ListUI(UITYPE::UI_SCREEN)->front())->Get_Floor();
	if (!Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
		m_bText = static_cast<CUIShop*>(Engine::Get_ListUI(UITYPE::UI_SHOP)->front())->Get_Finish();

	if (m_bText)
	{
		m_fWordTime += _fTimeDelta;
		if (m_fWordTime >= 0.2f)
		{
			m_fWordTime = 0.f;
			m_iWordCount++;

			if (m_iFloor == 1 && m_iWordCount >= 10) m_iWordCount = 10;
			else if (m_iFloor == 2 && m_iWordCount >= 11) m_iWordCount = 11;
			else if (m_iFloor == 3 && m_iWordCount >= 12) m_iWordCount = 12;
		}
	}

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIVictoryBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIVictoryBase::Render_Unit()
{
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_ICON]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_VICTORY::VICTORY_ICON]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_LIVE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_VICTORY::VICTORY_LIVE]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BALLOON]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BALLOON]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		if (m_bText)
		{
			_tchar Result[20];

			if (m_iFloor == 1)
				wcsncpy_s(Result, m_szComment1, m_iWordCount);
			else if (m_iFloor == 2)
				wcsncpy_s(Result, m_szComment2, m_iWordCount);
			else if (m_iFloor == 3)
				wcsncpy_s(Result, m_szComment3, m_iWordCount);

			Engine::Render_Font(L"Font_NotoSans", Result, &m_vTextPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}

		if (m_fAlpha[1] == 0.f) return;

		{
			DWORD tPreFactor;

			m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND]->Change_Alpha(m_fAlpha[0]);
			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND]->Set_Texture();
			Update_Color(m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND]);
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_VICTORY]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_VICTORY::VICTORY_VICTORY]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		if (m_fDelayTime >= 0.5f)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_OMEDETO]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_OMEDETO]->Set_Texture();
			m_pBufferCom->Render_Buffer();
		}

		if (m_fAlpha[1] > 0.f)
		{
			DWORD tPreFactor;

			m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);

			if (m_fAlpha[1] < 1.f)
			{
				m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Change_Alpha(m_fAlpha[1]);
				m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BAR]->Change_Alpha(m_fAlpha[1]);
				m_pTextureCom[(_uint)UI_VICTORY::VICTORY_FLOOR]->Change_Alpha(m_fAlpha[1]);
			}

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Get_WorldMatrix());


			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Set_Texture();
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BAR]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BAR]->Set_Texture();
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_FLOOR]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_FLOOR]->Set_Texture();
			m_pBufferCom->Render_Buffer();

			if (m_fDelayTime >= 1.f)
			{
				m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTextureTransformCom->Get_WorldMatrix());

				m_pGraphicDev->SetTexture(0, m_pTexture);
				m_pBufferCom->Render_Buffer();
			}

			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
		}

		{
			m_pGraphicDev->SetRenderTarget(0, m_pTextureSurface);
			m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DOT]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DOT]->Set_Texture();
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT1]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT1]->Set_Texture(m_iDigit[0]);
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT2]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT2]->Set_Texture(m_iDigit[1]);
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT3]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT3]->Set_Texture(m_iDigit[2]);
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT4]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT4]->Set_Texture(m_iDigit[3]);
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT5]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT5]->Set_Texture(m_iDigit[4]);
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_VICTORY::VICTORY_SEC]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_VICTORY::VICTORY_SEC]->Set_Texture();
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetRenderTarget(0, m_pPreSurface);

			m_bTextureRendered = true;
		}
	}
}

HRESULT CUIVictoryBase::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	{
		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_ICON] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Icon"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Icon", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_LIVE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Live"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Live", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BALLOON] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Balloon"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Balloon", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_BackGround"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_BackGround", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_VICTORY] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Victory"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Victory", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_OMEDETO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Omedeto"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Omedeto", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_FLOOR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Floor"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Floor", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BARBACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Grid__"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_BarBack", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_BAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Bar"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Bar", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Dot"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Dot", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DGreen"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DGreen"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit2", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT3] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DGreen"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit3", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT4] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DGreen"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit4", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_DIGIT5] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DGreen"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit5", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_VICTORY::VICTORY_SEC] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DGSec"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Sec", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_ICON] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Icon", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_LIVE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Live", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BALLOON] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Balloon", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_BackGround", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_VICTORY] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Victory", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_OMEDETO] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Omedeto", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_FLOOR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Floor", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BARBACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_BarBack", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Bar", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DOT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Dot", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit1", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit2", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT3] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit3", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT4] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit4", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT5] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit5", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_VICTORY::VICTORY_SEC] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Sec", pComponent });

		pComponent = m_pTextureTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Texture", pComponent });
	}

	return S_OK;
}

void CUIVictoryBase::Update_Color(CTexture* _pTexture)
{
	_float fHue = fmod(Engine::Get_Elapsed() * 70.f, 360.0f);
	_float fSaturation = 1.0f;
	_float fValue = 1.0f;

	_pTexture->Change_TextureColor(fHue, fSaturation, fValue);
}

void CUIVictoryBase::Update_FloorTime()
{
	_float iIntNumber = (int)m_fFloorTime;

	m_iDigit[0] = (int)(m_fFloorTime / 600);
	_int iNumber = (int)m_fFloorTime % 600;

	m_iDigit[1] = iNumber / 60;
	iNumber = iNumber % 60;

	m_iDigit[2] = iNumber / 10;
	iNumber = iNumber % 10;

	m_iDigit[3] = iNumber;
	iNumber = (int)((m_fFloorTime - iIntNumber) * 10.f);

	m_iDigit[4] = iNumber;
}

void CUIVictoryBase::Reset()
{
	m_bTextureRendered = false;
	m_bText = false;
	m_fSpeed = 10.f;
	m_fDelayTime = 0.f;
	m_fFloorTime = 0.f;
	m_fWordTime = 0.f;
	m_iFloor = 0;
	m_iWordCount = 0;
	m_fAlpha[0] = 0.f;
	m_fAlpha[1] = 1.f;

	for (_int i = 0; i < 3; ++i)
		m_fScaleRatio[i] = 5.f;

	for (_int i = 0; i < 5; ++i)
		m_iDigit[i] = 0;

	{
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_ICON]->Set_Pos(-WINCX / 2.f + 50.f, WINCY / 2.f - 50.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_LIVE]->Set_Pos(-WINCX / 2.f + 190.f, WINCY / 2.f - 50.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BALLOON]->Set_Pos(160.f, WINCY / 2.f - 100.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND]->Set_Pos(0.f, WINCY / 2.f - 150.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_VICTORY]->Set_Pos(0.f, WINCY / 2.f - 150.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_OMEDETO]->Set_Pos(0.f, WINCY / 2.f - 200.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Set_Pos(0.f, -WINCY / 2.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BAR]->Set_Pos(0.f, -WINCY / 2.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_FLOOR]->Set_Pos(0.f, 70.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DOT]->Set_Pos(0.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT1]->Set_Pos(-150.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT2]->Set_Pos(-100.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT3]->Set_Pos(-25.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT4]->Set_Pos(25.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT5]->Set_Pos(100.f, 0.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_SEC]->Set_Pos(170.f, -17.f, 0.f);
		m_pTextureTransformCom->Set_Pos(0.f, 0.f, 0.f);
	}

	{
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_ICON]->Set_Scale(25.f, 25.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_LIVE]->Set_Scale(110.f, 110.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BALLOON]->Set_Scale(500.f, 500.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BACKGROUND]->Set_Scale(300.f, 300.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_VICTORY]->Set_Scale(100.f, 100.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_OMEDETO]->Set_Scale(400.f, 400.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BARBACK]->Set_Scale(800.f, 500.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_BAR]->Set_Scale(WINCX, WINCY / 11.f - 10.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_FLOOR]->Set_Scale(200.f, 200.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DOT]->Set_Scale(80.f, 80.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT1]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT2]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT3]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT4]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_DIGIT5]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_VICTORY::VICTORY_SEC]->Set_Scale(40.f, 40.f, 0.f);
		m_pTextureTransformCom->Set_Scale(WINCX / 2.f, WINCY / 2.f, 0.f);
	}

	m_bRender = true;
}

void CUIVictoryBase::Free()
{
	m_pTextureSurface->Release();
	m_pPreSurface->Release();
	m_pTexture->Release();

	Engine::CUIUnit::Free();
}