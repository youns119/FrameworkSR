#pragma once

#include "Base.h"
#include "Define.h"
#include "Engine_Define.h"

class CLoading 
	: public CBase
{
public :
	enum class LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

private :
	explicit CLoading(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CLoading();

public :
	LOADINGID Get_LoadingID() { return m_eID; }
	CRITICAL_SECTION* Get_Crt() { return &m_tCrt; }
	_bool Get_Finish() { return m_bFinish; }
	const _tchar* Get_String() { return m_szLoading; }

public :
	static CLoading* Create(LPDIRECT3DDEVICE9 _pGraphicDev, LOADINGID _eID);

public :
	HRESULT	Ready_Loading(LOADINGID _eLoading);
	_uint Loading_Stage();

public :
	static unsigned int __stdcall Thread_Main(void* _pArg);

private :
	virtual void Free();

private :
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	_bool m_bFinish;

	HANDLE m_hThread;
	LOADINGID m_eID;

	CRITICAL_SECTION m_tCrt;

	_tchar m_szLoading[128];
};