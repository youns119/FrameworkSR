#include "pch.h"
#include "FloorCreate.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ToolView.h"

CFloorCreate::CFloorCreate()
{
	m_vecTile.reserve(TILEX * TILEY);
}


CFloorCreate::~CFloorCreate()
{
	Release();
}

HRESULT CFloorCreate::Initialize()
{

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Client/Bin/Resource/Texture/MMJ_Floor/Floor%d.png", TEX_MULTI, L"Floor", L"FloorTile", 10)))
	{
		AfxMessageBox(L"Tile Texture Failed");
		return E_FAIL;
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			// x좌표는 짝수 인 경우 지름 간격으로 타일의 중심점이 생성
			// y좌표는 홀수 인 경우 반지름 
			float	fX = (TILECX * j) + (i % 2) * (TILECX / 2.f);
			float	fY = (TILECY / 2.f) * i;

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY };
			pTile->byOption = 0; // 파일명 끝 숫자를 옵션 번호로 지정하도록 설정 하자
			pTile->byDrawID = 1;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

void CFloorCreate::Update()
{
}

void CFloorCreate::Render()
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR	szBuf[MIN_STR] = L"";
	int		iIndex(0);

	for (auto pTile : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x - m_MainView->GetScrollPos(0),
			pTile->vPos.y - m_MainView->GetScrollPos(1), 0.f);

		matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Floor", L"FloorTile", pTile->byDrawID);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, // 출력할 이미지 영역의 RECT 주소, NULL인 경우 0, 0 기준으로 출력
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 좌표, VEC3주소, NULL 인 경우 0, 0이 중심 좌표
			nullptr, // 텍스처를 출력할 위치 좌표, VEC3주소, 스크린상 0, 0 좌표에 출력
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 원본 이미지와 섞을 색상, 0xffffffff를 넘겨주면 원본색 유지


		////////////////타일 인덱스 출력//////////////////

		//swprintf_s(szBuf, L"%d", iIndex);

		////D3DXMATRIX	matTrans2;
		////D3DXMatrixTranslation(&matTrans2, 100.f, 100.f, 0.f);
		////CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans2);

		//CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		//++iIndex;
	}

}

void CFloorCreate::Release()
{
	
	/*for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<TILE*>);
	m_vecTile.clear();*/
}
