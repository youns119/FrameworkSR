#include "pch.h"
#include "..\Header\UIFloorBoss.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIFloorBoss::CUIFloorBoss(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_bTick(false)
	, m_fAlpha(1.f)
	, m_fTickTime(0.f)
	, m_fDuration(0.f)
	, m_fWordTime(0.f)
	, m_iWordIndex(0)
{
	for (_uint i = 0; i < (_uint)UI_FLOORBOSS::FLOORBOSS_END; i++)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUIFloorBoss::~CUIFloorBoss()
{
}

CUIFloorBoss* CUIFloorBoss::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIFloorBoss* pUIFloorBoss = new CUIFloorBoss(_pGraphicDev);

	if (FAILED(pUIFloorBoss->Ready_Unit()))
	{
		Safe_Release(pUIFloorBoss);
		MSG_BOX("UIFloorBoss create Failed");
		return nullptr;
	}

	return pUIFloorBoss;
}

HRESULT CUIFloorBoss::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	lstrcpy(m_szComment1, L"으아! 잠깐... 공주님이 가까이 있어요!");
	lstrcpy(m_szComment2, L"그러니까 일종의... 혼선이 발생했습니다!");
	lstrcpy(m_szComment3, L"평화 시스템이... 작동하지... 않네요...");
	lstrcpy(m_szComment4, L"아마... 지금... 오프라인인 것 같아요...");
	lstrcpy(m_szComment5, L"온라인 복구 시스템도 그렇고요...");
	lstrcpy(m_szComment6, L"지금부터는 남아있는 생명밖에 없어요! 조심하세요!");

	for (_int i = 0; i < 6; i++)
		m_iWordCount[i] = 0;

	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER]->Set_Pos(0.f, 100.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BAR]->Set_Pos(0.f, -30.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_NETWORK]->Set_Pos(0.f, -30.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIMELIMIT]->Set_Pos(0.f, -100.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIME]->Set_Pos(0.f, -160.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BALLOON]->Set_Pos(0.f, -WINCY / 2.f + 100.f, 0.f);

	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER]->Set_Scale(400.f, 400.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BAR]->Set_Scale(350.f, 350.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_NETWORK]->Set_Scale(230.f, 230.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIMELIMIT]->Set_Scale(100.f, 100.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIME]->Set_Scale(130.f, 130.f, 0.f);
	m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BALLOON]->Set_Scale(400.f, 400.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUIFloorBoss::Update_Unit(const _float& _fTimeDelta)
{
	m_fDuration += _fTimeDelta;
	//if (m_fDuration >= 10.f)
	//{
	//	m_pOwnerUI->Set_Render(false);
	//	return 0;
	//}

	m_fAlpha = 0.5f + fabs(0.5f * sinf(m_fDuration));

	m_fTickTime += _fTimeDelta;
	if (m_fTickTime >= 0.5f)
	{
		m_fTickTime = 0.f;
		m_bTick = !m_bTick;
	}

	if (m_iWordIndex == 0 && m_iWordCount[0] == 0)
		Play_Sound(L"Streamer_Boss1.wav", CHANNELID::SOUND_INTERFACE, 0.8f);
	else if (m_iWordIndex == 2 && m_iWordCount[2] == 0)
		Play_Sound(L"Streamer_Boss2.wav", CHANNELID::SOUND_INTERFACE, 0.8f);
	else if (m_iWordIndex == 4 && m_iWordCount[4] == 0)
		Play_Sound(L"Streamer_Boss3.wav", CHANNELID::SOUND_INTERFACE, 0.8f);

	m_fWordTime += _fTimeDelta;
	if (m_fWordTime >= 0.1f)
	{
		m_iWordCount[m_iWordIndex]++;
		m_fWordTime = 0.f;

		if (m_iWordIndex == 0 && m_iWordCount[0] >= 24)
		{
			m_iWordCount[0] = 24;
			m_iWordIndex++;
		}
		else if (m_iWordIndex == 1 && m_iWordCount[1] >= 24)
		{
			m_iWordCount[1] = 24;
			m_iWordIndex++;
		}
		else if (m_iWordIndex == 2 && m_iWordCount[2] >= 26)
		{
			m_iWordCount[2] = 26;
			m_iWordIndex++;
		}
		else if (m_iWordIndex == 3 && m_iWordCount[3] >= 27)
		{
			m_iWordCount[3] = 27;
			m_iWordIndex++;
		}
		else if (m_iWordIndex == 4 && m_iWordCount[4] >= 20)
		{
			m_iWordCount[4] = 20;
			m_iWordIndex++;
		}
		else if (m_iWordIndex == 5 && m_iWordCount[5] >= 28)
		{
			m_iWordCount[5] = 28;
			m_iWordIndex++;
		}
	}

	if (m_iWordIndex == 6)
	{
		m_pOwnerUI->Set_Render(false);
		return 0;
	}

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIFloorBoss::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIFloorBoss::Render_Unit()
{
	{
		DWORD tPreFactor;

		m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER]->Change_Alpha(m_fAlpha);
		m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BAR]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BAR]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_NETWORK]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_NETWORK]->Change_Alpha(m_fAlpha);
		m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_NETWORK]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIMELIMIT]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIMELIMIT]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	if (m_bTick)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIME]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIME]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BALLOON]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BALLOON]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	if (m_iWordIndex <= 5)
	{
		_vec2 vPos1 = { WINCX / 2.f - 300.f, WINCY - 140.f };
		_vec2 vPos2 = { WINCX / 2.f - 300.f, WINCY - 90.f };

		_tchar Result1[50];
		_tchar Result2[50];

		if (m_iWordIndex == 0 || m_iWordIndex == 1)
		{
			wcsncpy_s(Result1, m_szComment1, m_iWordCount[0]);
			wcsncpy_s(Result2, m_szComment2, m_iWordCount[1]);
		}
		else if (m_iWordIndex == 2 || m_iWordIndex == 3)
		{
			wcsncpy_s(Result1, m_szComment3, m_iWordCount[2]);
			wcsncpy_s(Result2, m_szComment4, m_iWordCount[3]);
		}
		else if (m_iWordIndex == 4 || m_iWordIndex == 5)
		{
			wcsncpy_s(Result1, m_szComment5, m_iWordCount[4]);
			wcsncpy_s(Result2, m_szComment6, m_iWordCount[5]);
		}

		Engine::Render_Font(L"Font_NotoSans", Result1, &vPos1, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Engine::Render_Font(L"Font_NotoSans", Result2, &vPos2, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
}

HRESULT CUIFloorBoss::Add_Component()
{
	CComponent* pComponent = NULL;

	{
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Danger"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Danger", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Bar"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Bar", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_NETWORK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Network"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Network", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIMELIMIT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Timelimit"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Timelimit", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIME] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Zero"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Time", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BALLOON] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIFloorBoss_Balloon"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Balloon", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_DANGER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Danger", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Bar", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_NETWORK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Network", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIMELIMIT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Timelimit", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_TIME] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Time", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_BALLOON] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Balloon", pComponent });
	}

	return S_OK;
}

void CUIFloorBoss::Reset()
{
}

void CUIFloorBoss::Free()
{
	Engine::CUIUnit::Free();
}