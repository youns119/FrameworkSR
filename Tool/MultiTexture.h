#pragma once
#include "Texture2.h"

class CMultiTexture : public CTexture2
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();
public:
	virtual const TEXINFO* Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0) override;
public:
	virtual HRESULT Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCnt = 0) override;
	virtual void Release() override;

private:
	map<wstring, vector<TEXINFO*>>		m_mapMultiTex;
};