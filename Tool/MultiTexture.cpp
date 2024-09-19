#include "pch.h"
#include "MultiTexture.h"
#include "Device.h"



CMultiTexture::CMultiTexture()
{
}

CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey, const int& iCnt)
{
	auto		iter = find_if(m_mapMultiTex.begin(), m_mapMultiTex.end(), [&](auto& MyPair)
		{
			if (pStateKey == MyPair.first)
				return true;

			return false;

		});

	if (iter == m_mapMultiTex.end())
		return nullptr;

	return iter->second[iCnt];
}

HRESULT CMultiTexture::Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey, const int& iCnt)
{
	TCHAR		szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCnt; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			Safe_Delete(pTexInfo);
			AfxMessageBox(L"D3DXGetImageInfoFromFile Failed");
			return E_FAIL;
		}


		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0,	// D3DUSAGE_RENDERTARGET : 화면 출력용 텍스처 생성 시 옵션
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT, // 이미지 필터링 방식
			D3DX_DEFAULT, // MIP필터 방식
			0,				// 제거할 색상, 0을 제거할 색상 컬러키 무효화
			nullptr,		// 이미지 정보기록 (m_pTexInfo->tImgInfo 위에서 이미 넣어주고 있음)
			nullptr,		// 팔레트 이미지 정보를 읽어들일 구조체 주소
			&(pTexInfo->pTexture))))
		{
			Safe_Delete(pTexInfo);
			AfxMessageBox(L"Multi Texture Create Failed");
			return E_FAIL;
		}

		m_mapMultiTex[pStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release()
{
	for_each(m_mapMultiTex.begin(), m_mapMultiTex.end(), [](auto& MyPair)
		{
			for_each(MyPair.second.begin(), MyPair.second.end(), Safe_Delete<TEXINFO*>);
			MyPair.second.clear();
		});

	m_mapMultiTex.clear();
}
