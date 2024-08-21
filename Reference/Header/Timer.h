#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTimer 
	: public CBase
{
private :
	explicit CTimer();
	virtual ~CTimer();

public :
	_float Get_TimeDelta() const { return m_fTimeDelta; }

public :
	static CTimer* Create();

public :
	HRESULT	Ready_Timer();
	void Update_Timer();

private :
	virtual void Free();

private :
	LARGE_INTEGER m_FrameTime;
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_FixTime;
	LARGE_INTEGER m_CpuTick;

	_float m_fTimeDelta;
};

END