#pragma once

class _declspec(dllexport) CBase
{
protected :
	inline explicit CBase();
	inline virtual ~CBase();

public :
	inline unsigned long AddRef();
	inline unsigned long Release();

public :
	inline virtual void	Free() = 0;

private :
	unsigned long m_dwRefCnt;
};

#include "Base.inl"