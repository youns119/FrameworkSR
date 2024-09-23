#pragma once
#include "SingleTexture.h"
#include "MultiTexture.h"


class CTextureMgr
{
	//mfc 때 경로를 통해 이미지를 불러옴 => mfc 툴에서는 이거 써야 하는지 확인 중

	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO* Get_Texture(const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);

public:
	HRESULT		Insert_Texture(const TCHAR* pFilePath, TEXTYPE eType, const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);
	void		Release();

private:
	map<wstring, CTexture2*>	m_mapTex;
};

