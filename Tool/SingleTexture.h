#pragma once
#include "Texture2.h"
class CSingleTexture : public CTexture2
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

public:
	virtual const TEXINFO* Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0) override
	{
		return m_pTexInfo;
	}

public:
	virtual HRESULT Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCnt = 0) override;
	virtual void Release() override;

private:
	TEXINFO* m_pTexInfo;
};

