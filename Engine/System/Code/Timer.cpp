#include "..\..\Header\Timer.h"

CTimer::CTimer()
    : m_fTimeDelta(0.f)
{
    ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
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
	QueryPerformanceCounter(&m_FrameTime);	// 1077
	QueryPerformanceCounter(&m_LastTime);	// 1085
	QueryPerformanceCounter(&m_FixTime);	// 1090

	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void CTimer::Update_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);	// 1500

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = (m_FrameTime.QuadPart - m_LastTime.QuadPart) / (_float)m_CpuTick.QuadPart;
	m_LastTime = m_FrameTime;
}

void CTimer::Free()
{
}