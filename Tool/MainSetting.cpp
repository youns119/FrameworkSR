#include "pch.h"
#include "MainSetting.h"
#include "Tool_Define.h"

#include "MainScene.h"

CMainSetting::CMainSetting()
	: m_pDeviceClass(nullptr)
	, m_pGraphicDev(nullptr)
	, m_pManagementClass(nullptr)
{
}

CMainSetting::~CMainSetting()
{
}

CMainSetting* CMainSetting::Create(HWND _hWnd)
{
	CMainSetting* pMainSetting = new CMainSetting;

	if (FAILED(pMainSetting->Ready_MainSetting(_hWnd)))
	{
		Safe_Release(pMainSetting);
		return nullptr;
	}

	return pMainSetting;
}

HRESULT CMainSetting::Ready_MainSetting(HWND _hWnd)
{

	m_hWnd = _hWnd;

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	return S_OK;
}

int CMainSetting::Update_MainSetting(const float& _fTimeDelta)
{
//	Engine::Update_InputDev();
	m_pManagementClass->Update_Scene(_fTimeDelta);

	return 0;
}

void CMainSetting::LateUpdate_MainSetting()
{
	m_pManagementClass->LateUpdate_Scene();
}

void CMainSetting::Render_MainSetting()
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 255.f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	Engine::Render_End();
}

HRESULT CMainSetting::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* _ppGraphicDev)
{

	if (FAILED(Engine::Ready_GraphicDev(m_hWnd, WINMODE::MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
	{
		return S_OK;
	}

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"궁서", 20, 20, FW_HEAVY)))
	{
		return S_OK;
	}

	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_Myungjo", L"명조", 20, 20, FW_NORMAL)))
	{
		return S_OK;
	}
	// 인스턴스 가져올 수 있도록 설정하기
	//if(FAILED(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	srand(unsigned int(time(NULL)));
	return S_OK;


}

HRESULT CMainSetting::Ready_Scene(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::CManagement** _ppManagement)
{
	
	Engine::CScene* pScene = nullptr;

	pScene = CMainScene::Create(_pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	if (FAILED(Engine::Create_Management(_pGraphicDev, _ppManagement)))
	{
		return E_FAIL;
	}
	(*_ppManagement)->AddRef();

	if (FAILED((*_ppManagement)->Set_Scene(pScene)))
	{
		return E_FAIL;

	}

	return S_OK;
}

void CMainSetting::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);

	Engine::Release_Utility();
	Engine::Release_System();
}
