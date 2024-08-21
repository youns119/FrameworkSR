#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerManager
	: public CBase
{
	DECLARE_SINGLETON(CTimerManager)

private :
	explicit CTimerManager();
	virtual ~CTimerManager();

public :
	_float Get_TimeDelta(const _tchar* pTimerTag);

	void Set_TimeDelta(const _tchar* pTimerTag);

public :
	HRESULT	Ready_Timer(const _tchar* pTimerTag);

private :
	CTimer* Find_Timer(const _tchar* pTimerTag);

private :
	virtual void Free();

private :
	map<const _tchar*, CTimer*> m_mapTimer;
};

END