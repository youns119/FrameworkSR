#include "pch.h"
#include "..\Header\UIScreenBase.h"
#include "..\Header\UIShop.h"
#include "..\Header\Player.h"
#include "Export_Utility.h"
#include "Export_System.h"

CUIScreenBase::CUIScreenBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTexture(nullptr)
	, m_pWideTexture(nullptr)
	, m_pWideSurface(nullptr)
	, m_pPreSurface(nullptr)
	, m_bReturn(false)
	, m_iFloor(1)
	, m_iPhase(0)
	, m_fBuzzTime(0.f)
{
	for (_uint i = 0; i < (_uint)UI_SCREENBASE::SCREENBASE_END - 2; i++)
		m_pTextureCom[i] = nullptr;

	for (_uint i = 0; i < (_uint)UI_SCREENBASE::SCREENBASE_END; i++)
		m_pTransformCom[i] = nullptr;

	for (_int i = 0; i < 5; i++)
		m_pAnimatorCom[i] = nullptr;

	for (_int i = 0; i < 2; ++i)
		m_bBackToNormal[i] = false;
}

CUIScreenBase::~CUIScreenBase()
{
}

CUIScreenBase* CUIScreenBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIScreenBase* pUIScreenBase = new CUIScreenBase(_pGraphicDev);

	if (FAILED(pUIScreenBase->Ready_Unit()))
	{
		Safe_Release(pUIScreenBase);
		MSG_BOX("UIScreenBase create Failed");
		return nullptr;
	}

	return pUIScreenBase;
}

HRESULT CUIScreenBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	{
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Set_Pos(WINCX * 0.1f + 22.f, -WINCY * 0.1f - 7.2f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME1]->Set_Pos(-WINCX * 0.1f + 10.f, WINCY * 0.1f - 10.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME2]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 195.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_SCREEN]->Set_Pos(-WINCX * 0.1f + 10.f, WINCY * 0.1f - 10.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FINE]->Set_Pos(-WINCX * 0.5f + 100.f, -WINCY * 0.5f + 65.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LINE]->Set_Pos(-100.f, -WINCY * 0.5f + 65.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LOGO]->Set_Pos(300.f, -WINCY * 0.5f + 65.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CHAT]->Set_Pos(WINCX * 0.5f - 123.f, -WINCY * 0.5f + 140.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FOLLOW]->Set_Pos(WINCX * 0.5f - 120.f, -65.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FLOOR]->Set_Pos(WINCX * 0.5f - 110.f, -52.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE1]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 195.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR1]->Set_Pos(WINCX * 0.5f - 123.f, WINCY * 0.5f - 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR2]->Set_Pos(WINCX * 0.5f - 110.f, WINCY * 0.5f - 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR3]->Set_Pos(WINCX * 0.5f - 120.f, WINCY * 0.5f - 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR4]->Set_Pos(WINCX * 0.5f - 118.f, WINCY * 0.5f - 110.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_INT]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 195.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_PEACE]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 27.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CLICKMUTE]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 350.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNUMBER]->Set_Pos(WINCX * 0.5f - 182.f, WINCY * 0.5f - 370.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNAME]->Set_Pos(WINCX * 0.5f - 90.f, WINCY * 0.5f - 367.f, 0.f);
	}

	{
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Set_Scale(WINCX / 2.f / 4.f * 5.f, WINCY / 2.f / 4.f * 5.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME1]->Set_Scale(560.f, 560.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME2]->Set_Scale(235.f, 228.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_SCREEN]->Set_Scale((WINCX / 2.f) * 0.8f, (WINCY / 2.f) * 0.8f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FINE]->Set_Scale(80.f, 80.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LINE]->Set_Scale(320.f, 320.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LOGO]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CHAT]->Set_Scale(147.f, 147.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FOLLOW]->Set_Scale(125.f, 125.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FLOOR]->Set_Scale(75.f, 75.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE1]->Set_Scale(220.f, 220.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR1]->Set_Scale(90.f, 90.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR2]->Set_Scale(100.f, 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR3]->Set_Scale(95.f, 95.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR4]->Set_Scale(122.f, 122.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_INT]->Set_Scale(90.f, 190.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_PEACE]->Set_Scale(90.f, 90.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CLICKMUTE]->Set_Scale(100.f, 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNUMBER]->Set_Scale(30.f, 30.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNAME]->Set_Scale(65.f, 65.f, 0.f);
	}

	m_bRender = true;

	Set_Animation();

	return S_OK;
}

_int CUIScreenBase::Update_Unit(const _float& _fTimeDelta)
{
	_vec3 vCurrPos;
	_vec3 vCurrScale;

	m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Get_Info(INFO::INFO_POS, &vCurrPos);
	vCurrScale = *m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Get_Scale();

	if (m_bReturn)
	{
		if (vCurrPos.x < WINCX * 0.1f + 22.f || vCurrPos.y > -WINCY * 0.1f - 7.2f)
		{
			_vec2 vPos =
			{
				vCurrPos.x + (WINCX * 0.1f + 22.f - vCurrPos.x) * _fTimeDelta * 6.f,
				vCurrPos.y + (-WINCY * 0.1f - 7.2f - vCurrPos.y) * _fTimeDelta * 6.f
			};

			if (fabs(WINCX * 0.1f + 22.f - vPos.x) <= 0.1f || fabs(-WINCY * 0.1f - 7.2f - vPos.y) <= 0.1f)
			{
				vPos = { WINCX * 0.1f + 22.f, -WINCY * 0.1f - 7.2f };
				m_bBackToNormal[0] = true;
			}

			m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Set_Pos(vPos.x, vPos.y, 0.f);
		}

		if (vCurrScale.x < WINCX / 2.f / 4.f * 5.f || vCurrScale.y < WINCY / 2.f / 4.f * 5.f)
		{
			_vec2 vScale =
			{
				vCurrScale.x + (WINCX / 2.f / 4.f * 5.f - vCurrScale.x) * _fTimeDelta * 5.5f,
				vCurrScale.y + (WINCY / 2.f / 4.f * 5.f - vCurrScale.y) * _fTimeDelta * 5.5f
			};

			if (fabs(WINCX / 2.f / 4.f * 5.f - vScale.x) <= 0.1f || fabs(WINCY / 2.f / 4.f * 5.f - vScale.y) <= 0.1f)
			{
				vScale = { WINCX / 2.f / 4.f * 5.f, WINCY / 2.f / 4.f * 5.f };
				m_bBackToNormal[1] = true;
			}

			m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Set_Scale(vScale.x, vScale.y, 0.f);
		}

		return Engine::CUIUnit::Update_Unit(_fTimeDelta);
	}

	if (vCurrPos.x > 0.f || vCurrPos.y < 0.f)
	{
		_vec2 vPos =
		{
			vCurrPos.x - vCurrPos.x * _fTimeDelta * 3.f,
			vCurrPos.y - vCurrPos.y * _fTimeDelta * 3.f
		};

		if (fabs(0.f - vPos.x) <= 0.1f || fabs(0.f - vPos.y) <= 0.1f) vPos = { 0.f, 0.f };

		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Set_Pos(vPos.x, vPos.y, 0.f);
	}

	if (vCurrScale.x > WINCX / 2.f || vCurrScale.y > WINCY / 2.f)
	{
		_vec2 vScale =
		{
			vCurrScale.x - (vCurrScale.x - WINCX / 2.f) * _fTimeDelta * 2.8f,
			vCurrScale.y - (vCurrScale.y - WINCY / 2.f) * _fTimeDelta * 2.8f
		};


		if (fabs(WINCX / 2.f - vScale.x) <= 0.1f || fabs(WINCY / 2.f - vScale.y) <= 0.1f)
			vScale = { WINCX / 2.f, WINCY / 2.f };

		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Set_Scale(vScale.x, vScale.y, 0.f);
	}

	if (!Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
	{
		if (static_cast<CUIShop*>(Engine::Get_ListUI(UITYPE::UI_SHOP)->front())->Get_Finish())
		{
			m_fBuzzTime += _fTimeDelta;

			if (m_iPhase == 0)
			{
				m_pAnimatorCom[2]->PlayAnimation(L"Static", true);

				m_iPhase = 1;
			}

			if (m_iPhase == 1 && m_fBuzzTime >= 0.5f)
			{
				if (m_iFloor == 1)
				{
					m_pAnimatorCom[2]->PlayAnimation(L"StreamerFace2", true);
					m_pAnimatorCom[3]->PlayAnimation(L"StreamerHair2", true);

					Play_Sound(L"Streamer_Floor1.wav", CHANNELID::SOUND_INTERFACE, 0.8f);
				}
				else if (m_iFloor == 2)
				{
					m_pAnimatorCom[2]->PlayAnimation(L"StreamerFace3", true);
					m_pAnimatorCom[3]->PlayAnimation(L"StreamerHair3", true);

					Play_Sound(L"Streamer_Floor2.wav", CHANNELID::SOUND_INTERFACE, 0.8f);
				}
				else if (m_iFloor == 3)
				{
					m_pAnimatorCom[2]->PlayAnimation(L"StreamerFace4", true);
					m_pAnimatorCom[3]->PlayAnimation(L"StreamerHair4", true);

					Play_Sound(L"Streamer_Floor3.wav", CHANNELID::SOUND_INTERFACE, 0.8f);
				}

				m_iPhase = 2;
			}
		}
	}

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIScreenBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIScreenBase::Render_Unit()
{
	{
		m_pGraphicDev->SetRenderTarget(0, m_pWideSurface);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME1]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME1]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME2]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME2]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_SCREEN]->Get_WorldMatrix());

		m_pGraphicDev->SetTexture(0, m_pTexture);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FINE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FINE]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LOGO]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_LOGO]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FOLLOW]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FOLLOW]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FLOOR]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FLOOR]->Set_Texture(m_iFloor - 1);
		Update_Color();
		m_pBufferCom->Render_Buffer();

		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LINE]->Get_WorldMatrix());

			m_pAnimatorCom[0]->Render_Animator();
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CHAT]->Get_WorldMatrix());

			m_pAnimatorCom[1]->Render_Animator();
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE1]->Get_WorldMatrix());

			m_pAnimatorCom[2]->Render_Animator();
			m_pBufferCom->Render_Buffer();

			if (m_iPhase == 0)
			{
				m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR1]->Get_WorldMatrix());

				m_pAnimatorCom[3]->Render_Animator();
				m_pBufferCom->Render_Buffer();
			}
			else if (m_iPhase == 2)
			{
				if (m_iFloor == 1)
				{
					m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR2]->Get_WorldMatrix());

					m_pAnimatorCom[3]->Render_Animator();
					m_pBufferCom->Render_Buffer();
				}
				else if (m_iFloor == 2)
				{
					m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR3]->Get_WorldMatrix());

					m_pAnimatorCom[3]->Render_Animator();
					m_pBufferCom->Render_Buffer();
				}
				else if (m_iFloor == 3)
				{
					m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR4]->Get_WorldMatrix());

					m_pAnimatorCom[3]->Render_Animator();
					m_pBufferCom->Render_Buffer();
				}
			}

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_INT]->Get_WorldMatrix());

			m_pAnimatorCom[4]->Render_Animator();
			m_pBufferCom->Render_Buffer();
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_PEACE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_PEACE]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CLICKMUTE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_CLICKMUTE]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNUMBER]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNUMBER]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNAME]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNAME]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderTarget(0, m_pPreSurface);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Get_WorldMatrix());

	m_pGraphicDev->SetTexture(0, m_pWideTexture);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIScreenBase::Add_Component()
{
	CComponent* pComponent = NULL;

	{
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Grid"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Frame1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Grid_"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Frame2", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FINE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Fine"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Fine", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_LINE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Line"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Line", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_LOGO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Logo"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Logo", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_CHAT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Chat"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Chat", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FOLLOW] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Follow"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Follow", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FLOOR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Floor"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Floor", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STATIC] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Static"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Static", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Streamer_Face1"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Streamer_Face1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Streamer_Face2"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Streamer_Face2", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE3] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Streamer_Face3"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Streamer_Face3", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE4] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Streamer_Face4"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Streamer_Face4", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Streamer_Hair1"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Streamer_Hair1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Streamer_Hair2"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Streamer_Hair2", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR3] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Streamer_Hair3"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Streamer_Hair3", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR4] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Streamer_Hair4"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Streamer_Hair4", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_INT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_INT"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Int", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_PEACE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_Peace"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Peace", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_CLICKMUTE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_ClickMute"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_ClickMute", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNUMBER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_StreamerNumber"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_StreamerNumber", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNAME] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIScreen_StreamerName"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_StreamerName", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Wide", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_SCREEN] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Screen", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Frame1", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Frame2", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FINE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Fine", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LINE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Line", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LOGO] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Logo", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CHAT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Chat", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FOLLOW] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Follow", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FLOOR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Floor", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Streamer_Face", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Streamer_Hair1", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Streamer_Hair2", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR3] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Streamer_Hair3", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR4] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Streamer_Hair4", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_INT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Int", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_PEACE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Peace", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CLICKMUTE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_ClickMute", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNUMBER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_StreamerNumber", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNAME] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_StreamerName", pComponent });
	}

	{
		pComponent = m_pAnimatorCom[0] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Line", pComponent });

		pComponent = m_pAnimatorCom[1] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Chat", pComponent });

		pComponent = m_pAnimatorCom[2] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Streamer_Face1", pComponent });

		pComponent = m_pAnimatorCom[3] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Streamer_Hair1", pComponent });

		pComponent = m_pAnimatorCom[4] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Int", pComponent });
	}

	m_pTexture = Engine::Get_ChangedTexture();
	m_pPreSurface = Engine::Get_PreSurface();

	m_pGraphicDev->CreateTexture
	(
		WINCX, WINCY,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pWideTexture,
		NULL
	);

	m_pWideTexture->GetSurfaceLevel(0, &m_pWideSurface);

	return S_OK;
}

void CUIScreenBase::Set_Animation()
{
	m_pAnimatorCom[0]->CreateAnimation(L"Line", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_LINE], 20.f);
	m_pAnimatorCom[0]->PlayAnimation(L"Line", true);

	m_pAnimatorCom[1]->CreateAnimation(L"Chat", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_CHAT], 10.f);
	m_pAnimatorCom[1]->PlayAnimation(L"Chat", true);

	m_pAnimatorCom[2]->CreateAnimation(L"Static", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STATIC], 10.f);
	m_pAnimatorCom[2]->CreateAnimation(L"StreamerFace1", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE1], 8.f);
	m_pAnimatorCom[2]->CreateAnimation(L"StreamerFace2", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE2], 8.f);
	m_pAnimatorCom[2]->CreateAnimation(L"StreamerFace3", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE3], 8.f);
	m_pAnimatorCom[2]->CreateAnimation(L"StreamerFace4", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE4], 8.f);
	m_pAnimatorCom[2]->PlayAnimation(L"StreamerFace1", true);

	m_pAnimatorCom[3]->CreateAnimation(L"StreamerHair1", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR1], 10.f);
	m_pAnimatorCom[3]->CreateAnimation(L"StreamerHair2", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR2], 10.f);
	m_pAnimatorCom[3]->CreateAnimation(L"StreamerHair3", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR3], 10.f);
	m_pAnimatorCom[3]->CreateAnimation(L"StreamerHair4", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR4], 10.f);
	m_pAnimatorCom[3]->PlayAnimation(L"StreamerHair1", true);

	m_pAnimatorCom[4]->CreateAnimation(L"Int", m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_INT], 7.f);
	m_pAnimatorCom[4]->PlayAnimation(L"Int", true);
}

void CUIScreenBase::Update_Color()
{
	_float fHue = fmod(Engine::Get_Elapsed() * 70.f, 360.0f);
	_float fSaturation = 1.0f;
	_float fValue = 1.0f;

	m_pTextureCom[(_uint)UI_SCREENBASE::SCREENBASE_FLOOR]->Change_TextureColor(fHue, fSaturation, fValue);
}

void CUIScreenBase::Reset()
{
	m_bReturn = false;
	m_iFloor = 0;
	m_iPhase = 0;
	m_fBuzzTime = 0.f;

	for (_int i = 0; i < 2; ++i)
		m_bBackToNormal[i] = false;

	{
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Set_Pos(WINCX * 0.1f + 22.f, -WINCY * 0.1f - 7.2f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME1]->Set_Pos(-WINCX * 0.1f + 10.f, WINCY * 0.1f - 10.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME2]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 195.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_SCREEN]->Set_Pos(-WINCX * 0.1f + 10.f, WINCY * 0.1f - 10.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FINE]->Set_Pos(-WINCX * 0.5f + 100.f, -WINCY * 0.5f + 65.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LINE]->Set_Pos(-100.f, -WINCY * 0.5f + 65.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LOGO]->Set_Pos(300.f, -WINCY * 0.5f + 65.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CHAT]->Set_Pos(WINCX * 0.5f - 123.f, -WINCY * 0.5f + 140.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FOLLOW]->Set_Pos(WINCX * 0.5f - 120.f, -65.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FLOOR]->Set_Pos(WINCX * 0.5f - 110.f, -52.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE1]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 195.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR1]->Set_Pos(WINCX * 0.5f - 123.f, WINCY * 0.5f - 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR2]->Set_Pos(WINCX * 0.5f - 110.f, WINCY * 0.5f - 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR3]->Set_Pos(WINCX * 0.5f - 120.f, WINCY * 0.5f - 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR4]->Set_Pos(WINCX * 0.5f - 118.f, WINCY * 0.5f - 110.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_INT]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 195.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_PEACE]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 27.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CLICKMUTE]->Set_Pos(WINCX * 0.5f - 117.f, WINCY * 0.5f - 350.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNUMBER]->Set_Pos(WINCX * 0.5f - 182.f, WINCY * 0.5f - 370.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNAME]->Set_Pos(WINCX * 0.5f - 90.f, WINCY * 0.5f - 367.f, 0.f);
	}

	{
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_WIDE]->Set_Scale(WINCX / 2.f / 4.f * 5.f, WINCY / 2.f / 4.f * 5.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME1]->Set_Scale(560.f, 560.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FRAME2]->Set_Scale(235.f, 228.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_SCREEN]->Set_Scale((WINCX / 2.f) * 0.8f, (WINCY / 2.f) * 0.8f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FINE]->Set_Scale(80.f, 80.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LINE]->Set_Scale(320.f, 320.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_LOGO]->Set_Scale(40.f, 40.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CHAT]->Set_Scale(147.f, 147.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FOLLOW]->Set_Scale(125.f, 125.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_FLOOR]->Set_Scale(75.f, 75.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_FACE1]->Set_Scale(220.f, 220.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR1]->Set_Scale(90.f, 90.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR2]->Set_Scale(100.f, 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR3]->Set_Scale(95.f, 95.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMER_HAIR4]->Set_Scale(122.f, 122.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_INT]->Set_Scale(90.f, 190.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_PEACE]->Set_Scale(90.f, 90.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_CLICKMUTE]->Set_Scale(100.f, 100.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNUMBER]->Set_Scale(30.f, 30.f, 0.f);
		m_pTransformCom[(_uint)UI_SCREENBASE::SCREENBASE_STREAMERNAME]->Set_Scale(65.f, 65.f, 0.f);
	}

	m_pAnimatorCom[2]->PlayAnimation(L"StreamerFace1", true);
	m_pAnimatorCom[3]->PlayAnimation(L"StreamerHair1", true);
}

void CUIScreenBase::Free()
{
	Engine::CUIUnit::Free();
}