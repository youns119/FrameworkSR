#include "..\..\Header\TimerManager.h"

IMPLEMENT_SINGLETON(CTimerManager)

CTimerManager::CTimerManager()
	: m_pCurrTimer(nullptr)
{
	m_mapTimer.clear();
}

CTimerManager::~CTimerManager()
{
	Free();
}

_float CTimerManager::Get_TimeDelta(const _tchar* _pTimerTag)
{
	CTimer* pTimer = Find_Timer(_pTimerTag);
	NULL_CHECK_RETURN(pTimer, 0.f);

	return pTimer->Get_TimeDelta();
}

void CTimerManager::Set_TimeDelta(const _tchar* _pTimerTag)
{
	m_pCurrTimer = Find_Timer(_pTimerTag);
	NULL_CHECK(m_pCurrTimer);

	m_pCurrTimer->Update_Timer();
}

HRESULT CTimerManager::Ready_Timer(const _tchar* _pTimerTag)
{
	CTimer* pTimer = Find_Timer(_pTimerTag);

	if (pTimer != nullptr)
		return E_FAIL;

	pTimer = CTimer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	m_mapTimer.insert({ _pTimerTag, pTimer });

	return S_OK;
}

void CTimerManager::Stop_Timer(_bool _bStop)
{
	m_pCurrTimer->Stop_Timer(_bStop);
}

CTimer* CTimerManager::Find_Timer(const _tchar* _pTimerTag)
{
	auto iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CTag_Finder(_pTimerTag));

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

void CTimerManager::Free()
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CDeleteMap());
	m_mapTimer.clear();
}