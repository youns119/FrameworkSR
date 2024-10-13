#include "pch.h"
#include "..\Header\Logo.h"
#include "..\Header\Stage.h"
#include "..\Header\MapCreate.h"
#include "Export_System.h"
#include "Export_Utility.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
	, m_pLoading(nullptr)
	, m_hVideoHandle(nullptr)
	, m_bVideoPlaying(false)
{
}

CLogo::~CLogo()
{
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CLogo* pLogo = new CLogo(_pGraphicDev);

	if (FAILED(pLogo->Ready_Scene()))
	{
		Safe_Release(pLogo);
		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pLogo;
}

HRESULT CLogo::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADINGID::LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	//FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);

	PlayVideo(g_hWnd, L"../Bin/Resource/Texture/MMJ_Interface/CutScene/Opening_Sound.wmv");

	return S_OK;
}

_int CLogo::Update_Scene(const _float& _fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(_fTimeDelta);

	Engine::CScene* pStage = CStage::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pStage, -1);

	FAILED_CHECK_RETURN(Engine::Set_Scene(pStage), E_FAIL);

	//if (m_pLoading->Get_Finish() == true)
	//{
	//	if (Engine::Key_Press(DIK_RETURN))
	//	{
	//		Engine::CScene* pStage = CStage::Create(m_pGraphicDev);
	//		NULL_CHECK_RETURN(pStage, -1);
	//
	//		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage), E_FAIL);
	//
	//		return 0;
	//	}
	//	if (Engine::Key_Press(DIK_SPACE))
	//	{
	//		Engine::CScene* pMapCreate = CMapCreate::Create(m_pGraphicDev);
	//		NULL_CHECK_RETURN(pMapCreate, -1);
	//
	//		FAILED_CHECK_RETURN(Engine::Set_Scene(pMapCreate), E_FAIL);
	//
	//		return 0;
	//	}
	//}

	return iExit;
}

void CLogo::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	_vec2 vPos = { 100.f, 100.f };

	Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CLogo::Ready_Prototype()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Loading", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Logo/Ending/%d.png", TEXTUREID::TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Animator", Engine::CAnimator::Create(m_pGraphicDev)), E_FAIL);

	return S_OK;
}

void CLogo::PlayVideo(HWND _hWnd, const wstring& _strFilePath)
{
	if (m_bVideoPlaying)
		return;

	m_hVideoHandle = MCIWndCreate(_hWnd,
		NULL,
		WS_CHILD |
		WS_VISIBLE |
		MCIWNDF_NOPLAYBAR, _strFilePath.c_str());

	if (m_hVideoHandle == NULL)
	{
		MessageBox(_hWnd, L"Fail Create Video.", L"Error", MB_OK);
		return;
	}

	MoveWindow(m_hVideoHandle, 0, 0, WINCX, 720, FALSE);

	m_bVideoPlaying = true;
	MCIWndPlay(m_hVideoHandle);
	HDC dc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(dc);
	HBITMAP hBitmap = CreateCompatibleBitmap(dc, WINCX, WINCY);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

	Rectangle(dc, 0, 0, WINCX, WINCY);
	BitBlt(dc, 0, 0, WINCX, WINCY, memDC, 0, 0, SRCCOPY);
	while (MCIWndGetLength(m_hVideoHandle) > MCIWndGetPosition(m_hVideoHandle))
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			MCIWndClose(m_hVideoHandle);
			m_bVideoPlaying = false;
			break;
		}

	}

	SelectObject(memDC, hOldBitmap);
	ReleaseDC(_hWnd, memDC);
	ReleaseDC(_hWnd, dc);

	m_bVideoPlaying = false;

	MCIWndClose(m_hVideoHandle);
}

HRESULT CLogo::Ready_Layer_Environment(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

void CLogo::Free()
{
	Safe_Release(m_pLoading);

	Engine::CScene::Free();
}