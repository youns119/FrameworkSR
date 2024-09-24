#include "pch.h"
#include "..\Header\MainApp.h"

CMainApp::CMainApp()
	: m_pDeviceClass(nullptr)
	, m_pGraphicDev(nullptr)
	, m_pManagementClass(nullptr)
{
}

CMainApp::~CMainApp()
{
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	if (FAILED(pMainApp->Ready_MainApp()))
	{
		Safe_Release(pMainApp);
		return nullptr;
	}

	return pMainApp;
}

HRESULT CMainApp::Ready_MainApp()
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	Engine::Ready_SoundManager();

	return S_OK;
}

int CMainApp::Update_MainApp(const float& _fTimeDelta)
{
	Engine::Update_InputDev();

	m_pManagementClass->Update_Scene(_fTimeDelta);
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	return 0;
}

void CMainApp::LateUpdate_MainApp()
{
	m_pManagementClass->LateUpdate_Scene();
}

void CMainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();

	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* _ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, WINMODE::MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	(*_ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*_ppGraphicDev)->AddRef();

	(*_ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	(*_ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);
	(*_ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	FAILED_CHECK_RETURN(Engine::Ready_Font(*_ppGraphicDev, L"Font_Default", L"궁서", 20, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(*_ppGraphicDev, L"Font_Myungjo", L"명조", 20, 20, FW_NORMAL), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	(*_ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*_ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//imgui 초기화 구문
	IMGUI_CHECKVERSION(); // 버전 체크
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(*_ppGraphicDev);

	srand(unsigned int(time(NULL)));

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::CManagement** _ppManagement)
{
	Engine::CScene* pScene = nullptr;

	pScene = CLogo::Create(_pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(_pGraphicDev, _ppManagement), E_FAIL);
	(*_ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*_ppManagement)->Set_Scene(pScene), E_FAIL);

	return S_OK;
}

void CMainApp::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);
	
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
	Engine::Release_Utility();
	Engine::Release_System();


}