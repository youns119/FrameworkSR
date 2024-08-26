#pragma once

#include "Base.h"
#include "Frame.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFrameManager 
	: public CBase
{
	DECLARE_SINGLETON(CFrameManager)

private :
	explicit CFrameManager();
	virtual ~CFrameManager();

public :
	HRESULT	Ready_Frame(const _tchar* _pFrameTag, const _float& _fCallLimit);
	_bool IsPermit_Call(const _tchar* _pFrameTag, const _float& _fTimeDelta);

private :
	CFrame* Find_Frame(const _tchar* _pFrameTag);

private :
	virtual void Free();

private :
	map<const _tchar*, CFrame*>	m_mapFrame;
};

END