#include "pch.h"
#include "../Header/EndingScene.h"
#include "Export_System.h"
#include "Export_Utility.h"


CEndingScene::CEndingScene(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
	, m_hVideoHandle(nullptr)
	, m_bVideoPlaying(false)
{
}

CEndingScene::~CEndingScene()
{
}

CEndingScene* CEndingScene::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEndingScene* pLogo = new CEndingScene(_pGraphicDev);

	if (FAILED(pLogo->Ready_Scene()))
	{
		Safe_Release(pLogo);
		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pLogo;
}

HRESULT CEndingScene::Ready_Scene()
{
	CCollisionManager::GetInstance()->Free();
	CBulletManager::GetInstance()->Free();
	CTrigger::GetInstance()->Free();
	CDrinkSpawner::GetInstance()->Free();

	Engine::Deactivate_UI(UITYPE::UI_PHONE);
	Engine::Deactivate_UI(UITYPE::UI_SHOP);
	Engine::Deactivate_UI(UITYPE::UI_PHONE);
	Engine::Deactivate_UI(UITYPE::UI_MISTERBULLET);
	Engine::Deactivate_UI(UITYPE::UI_ROBOTO);
	Engine::Deactivate_UI(UITYPE::UI_PLAYER);
	Engine::Deactivate_UI(UITYPE::UI_DOWN);
	Engine::Deactivate_UI(UITYPE::UI_ACTION);
	Engine::Deactivate_UI(UITYPE::UI_INVENTORY);
	Engine::Deactivate_UI(UITYPE::UI_NORMAL);
	Engine::Deactivate_UI(UITYPE::UI_FREECAM);
	Engine::Deactivate_UI(UITYPE::UI_SCREEN);
	Engine::Deactivate_UI(UITYPE::UI_SKILL);

	Engine::Stop_All();


	PlayVideo(g_hWnd, L"../Bin/Resource/Texture/MMJ_Interface/CutScene/Ending_Sound.wmv");

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);

	return S_OK;
}

_int CEndingScene::Update_Scene(const _float& _fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(_fTimeDelta);


	return iExit;
}

void CEndingScene::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();
}

void CEndingScene::Render_Scene()
{
}



void CEndingScene::PlayVideo(HWND _hWnd, const wstring& _strFilePath)
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

HRESULT CEndingScene::Ready_Layer_Environment(const _tchar* _pLayerTag)
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

void CEndingScene::Free()
{
	Engine::CScene::Free();
}