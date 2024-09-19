#pragma once
#include "Tool_Include.h"

class CTexture2
{
public:
	CTexture2();
	virtual ~CTexture2();

public:
	virtual const TEXINFO* Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0)PURE;

public:
	virtual HRESULT		Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCnt = 0)PURE;
	virtual void		Release()PURE;

};

