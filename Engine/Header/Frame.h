#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFrame 
	: public CBase
{
private :
	explicit CFrame();
	virtual ~CFrame();

public :
	static CFrame* Create(const _float& _fCallLimit);

public :
	HRESULT	Ready_Frame(const _float& _fCallLimit);
	_bool IsPermit_Call(const _float& _fTimeDelta);

private :
	virtual void Free();

private :
	_float m_fCallLimit;
	_float m_fAccTimeDelta;
};

END