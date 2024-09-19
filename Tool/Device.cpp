
#include "pch.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr), m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{
}

CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::Init_Device()
{
	// 1. 그래픽 카드의 성능을 조사할 객체를 먼저 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 2. 실제 그래픽 카드의 성능 조사 명령
	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// FAILED : 성공 유무를 확인하는 매크로
	// D3DADAPTER_DEFAULT : 정보를 얻으려는 기본 그래픽 카드 의미

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD	vp(0);

	// 버텍스 프로세싱 = 정점의 변환 + 조명 연산

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 3. 그래픽 카드를 제어할 객체 생성

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	// CreateDevice : 디바이스 객체 생성 함수
	// CreateDevice(어떤 그래픽 카드를 제어할 것인가, 어떤 정보를 토대로 장치에 접근할 것인가, 사용할 윈도우 핸들, 동작 방식, 사용 환경, 생성할 컴객체)

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Width = 10;
	tFontInfo.Height = 20;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");


	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"D3DXCreateFontIndirect Failed");
		return E_FAIL;
	}

	// E_FAIL
	return S_OK;
}

void CDevice::Render_Begin()
{
	m_pDevice->Clear(0,			// 렉트의 개수
		nullptr,	// 렉트 배열의 첫 번째 주소
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 255, 255, 255), // 백 버퍼 색상
		1.f, // z버퍼 초기화 값
		0);	 // 스텐실 버퍼 초기화

	m_pDevice->BeginScene();

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();

	m_pDevice->EndScene();

	// 1, 2번, 4번 : 스왑 체인이 D3DSWAPEFFECT_COPY가 아니면 NULL
	// 3번 : 출력 대상의 창 핸들
	m_pDevice->Present(NULL, NULL, hWnd, NULL);
}

void CDevice::Release()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD  : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP	  :	버퍼 하나를 뒤집으면서 사용하는 방식
	// D3DSWAPEFFECT_COPY	  : 더블 버퍼링과 유사한 복사 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = g_hWnd;		// 디바이스를 사용할 창 핸들 지정

	d3dpp.Windowed = TRUE;		// TRUE는 창 모드 FALSE는 전체화면 모드

	d3dpp.EnableAutoDepthStencil = TRUE;	// 자동으로 깊이, 스텐실 버퍼를 관리
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 전체화면 출력 시에만 사용됨

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격을 장치가 설정
	// D3DPRESENT_INTERVAL_IMMEDIATE : 즉시 시연

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
