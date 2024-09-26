#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerManager
	: public CBase
{
	DECLARE_SINGLETON(CTimerManager)

private:
	explicit CTimerManager();
	virtual ~CTimerManager();

public:
	_float Get_TimeDelta(const _tchar* _pTimerTag);
	_bool Get_Stop() { return m_pCurrTimer->Get_Stop(); }

	void Set_TimeDelta(const _tchar* _pTimerTag);

public:
	HRESULT	Ready_Timer(const _tchar* _pTimerTag);

public:
	void Stop_Timer(_bool _bStop);

private:
	CTimer* Find_Timer(const _tchar* _pTimerTag);

private:
	virtual void Free();

private:
	map<const _tchar*, CTimer*> m_mapTimer;

	CTimer* m_pCurrTimer;
};

END