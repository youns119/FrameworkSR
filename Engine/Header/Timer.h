#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTimer
	: public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	_float Get_TimeDelta() const { return m_fTimeDelta; }
	_bool Get_Stop() const { return m_bStop; }

public:
	static CTimer* Create();

public:
	HRESULT	Ready_Timer();
	void Update_Timer();

public:
	void Stop_Timer(_bool _bStop) { m_bStop = _bStop; }

private:
	virtual void Free();

private:
	LARGE_INTEGER m_tFrameTime;
	LARGE_INTEGER m_tLastTime;
	LARGE_INTEGER m_tFixTime;
	LARGE_INTEGER m_tCpuTick;

	_float m_fTimeDelta;

	_bool m_bStop;
};

END