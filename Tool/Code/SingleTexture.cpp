#include "pch.h"
#include "SingleTexture.h"
#include "Device.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::Insert_Texture(const TCHAR* pFilePath,
	const TCHAR* pStateKey,
	const int& iCnt)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(L"D3DXGetImageInfoFromFile Failed");
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT : 가장 적합한 메모리에 보관, 보통 그래픽 카드 메모리를 말함
	// D3DPOOL_MANAGED : 그래픽 메모리를 사용하지만 이를 RAM에 백업
	// 
	// D3DPOOL_SYSTEMMEM : 메인 메모리(RAM)에 보관
	// D3DPOOL_SCRATCH : 시스템 메모리에 저장하지만 DX 장치가 접근 불가

	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
		pFilePath,
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0,	// D3DUSAGE_RENDERTARGET : 화면 출력용 텍스처 생성 시 옵션
		m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT, // 이미지 필터링 방식
		D3DX_DEFAULT, // MIP필터 방식
		0,				// 제거할 색상, 0을 제거할 색상 컬러키 무효화
		nullptr,		// 이미지 정보기록 (m_pTexInfo->tImgInfo 위에서 이미 넣어주고 있음)
		nullptr,		// 팔레트 이미지 정보를 읽어들일 구조체 주소
		&(m_pTexInfo->pTexture))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(L"Single Texture Create Failed");
		return E_FAIL;
	}
	return S_OK;
}

void CSingleTexture::Release()
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
