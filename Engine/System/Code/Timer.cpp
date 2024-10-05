#include "..\..\Header\Timer.h"

CTimer::CTimer()
	: m_fTimeDelta(0.f)
	, m_fElapsedTime(0.f)
	, m_fElapsedTime_Fixed(0.f)
	, m_bStop(false)
{
	ZeroMemory(&m_tFrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_tLastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_tFixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_tCpuTick, sizeof(LARGE_INTEGER));
}

CTimer::~CTimer()
{
}

CTimer* CTimer::Create()
{
	CTimer* pInstance = new CTimer;

	if (FAILED(pInstance->Ready_Timer()))
	{
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_tFrameTime);	// 1077
	QueryPerformanceCounter(&m_tLastTime);	// 1085
	QueryPerformanceCounter(&m_tFixTime);	// 1090

	QueryPerformanceFrequency(&m_tCpuTick);

	return S_OK;
}

void CTimer::Update_Timer()
{
	QueryPerformanceCounter(&m_tFrameTime);	// 1500

	if (m_tFrameTime.QuadPart - m_tFixTime.QuadPart >= m_tCpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_tCpuTick);
		m_tFixTime = m_tFrameTime;
	}

	m_fTimeDelta = (m_tFrameTime.QuadPart - m_tLastTime.QuadPart) / (_float)m_tCpuTick.QuadPart;
	m_tLastTime = m_tFrameTime;

	m_fElapsedTime_Fixed += m_fTimeDelta;

	if (m_bStop)
		m_fTimeDelta = 0.f;

	m_fElapsedTime += m_fTimeDelta;
}

void CTimer::Free()
{
}