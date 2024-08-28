#include "pch.h"
#include "..\Header\Loading.h"
#include "Export_Utility.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
	, m_bFinish(false)
	, m_hThread(nullptr)
	, m_eID(LOADINGID::LOADING_END)
{
	ZeroMemory(&m_tCrt, sizeof(CRITICAL_SECTION));

	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 _pGraphicDev, LOADINGID _eID)
{
	CLoading* pLoading = new CLoading(_pGraphicDev);

	if (FAILED(pLoading->Ready_Loading(_eID)))
	{
		Safe_Release(pLoading);
		return nullptr;
	}

	return pLoading;
}

HRESULT CLoading::Ready_Loading(LOADINGID _eLoading)
{
	InitializeCriticalSection(&m_tCrt);

	m_eID = _eLoading;

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	return S_OK;
}

_uint CLoading::Loading_Stage()
{
	// Etc
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);

	// Buffer
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev)), E_FAIL);

	// Texture
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Ma.jpg", TEXTUREID::TEX_NORMAL)), E_FAIL);

	m_bFinish = true;

	return 0;
}

unsigned int __stdcall CLoading::Thread_Main(void* _pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(_pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADINGID::LOADING_STAGE:
		iFlag = pLoading->Loading_Stage();
		break;

	case LOADINGID::LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}