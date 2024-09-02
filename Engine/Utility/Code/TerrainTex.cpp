#include "..\..\Header\TerrainTex.h"

CTerrainTex::CTerrainTex()
	: m_hFile(nullptr)
{
	ZeroMemory(&m_tFH, sizeof(BITMAPFILEHEADER));
	ZeroMemory(&m_tIH, sizeof(BITMAPINFOHEADER));
}

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CVIBuffer(_pGraphicDev)
	, m_hFile(nullptr)
{
	ZeroMemory(&m_tFH, sizeof(BITMAPFILEHEADER));
	ZeroMemory(&m_tIH, sizeof(BITMAPINFOHEADER));
}

CTerrainTex::CTerrainTex(const CTerrainTex& _rhs)
	: CVIBuffer(_rhs)
	, m_hFile(_rhs.m_hFile)
	, m_tFH(_rhs.m_tFH)
	, m_tIH(_rhs.m_tIH)
{
}

CTerrainTex::~CTerrainTex()
{
}

CTerrainTex* CTerrainTex::Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv)
{
	CTerrainTex* pInstance = new CTerrainTex(_pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(_dwCntX, _dwCntZ, _dwVtxItv)))
	{
		Safe_Release(pInstance);
		MSG_BOX("TerrainTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CTerrainTex::Ready_Buffer(const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv)
{
	m_dwVtxCnt = _dwCntX * _dwCntZ;
	m_dwTriCnt = (_dwCntX - 1) * (_dwCntZ - 1) * 2;

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_eIdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	m_hFile = CreateFile
	(
		L"../Bin/Resource/Texture/Terrain/Height1.bmp", 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL
	);

	if (INVALID_HANDLE_VALUE == m_hFile)
		MSG_BOX("높이맵 텍스처 개방 실패");

	_ulong	dwByte(0);

	ReadFile(m_hFile, &m_tFH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_tIH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	_ulong* pPixel = new _ulong[m_tIH.biWidth * m_tIH.biHeight];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_tIH.biWidth * m_tIH.biHeight, &dwByte, NULL);

	CloseHandle(m_hFile);

	VTXTEX* pVertex = NULL;

	_ulong	dwIndex(0);

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < _dwCntZ; i++)
	{
		for (_ulong j = 0; j < _dwCntX; j++)
		{
			dwIndex = i * _dwCntX + j;

			pVertex[dwIndex].vPosition = _vec3
			(
				_float(j) * _dwVtxItv,
				_float(pPixel[dwIndex] & 0x000000ff) / 20.f,
				_float(i) * _dwVtxItv
			);
			pVertex[dwIndex].vTexUV = _vec2
			(
				(_float(j) / (_dwCntX - 1)) * 20.f,
				(_float(i) / (_dwCntZ - 1)) * 20.f
			);
		}
	}

	m_pVB->Unlock();

	Safe_Delete_Array(pPixel);

	INDEX32* pIndex = nullptr;
	_ulong dwTriCnt(0);

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < _dwCntZ - 1; i++)
	{
		for (_ulong j = 0; j < _dwCntX - 1; j++)
		{
			dwIndex = i * _dwCntX + j;

			pIndex[dwTriCnt]._0 = dwIndex + _dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + _dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;
			dwTriCnt++;

			pIndex[dwTriCnt]._0 = dwIndex + _dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
			dwTriCnt++;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CTerrainTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CTerrainTex::Clone()
{
	return new CTerrainTex(*this);
}

void CTerrainTex::Free()
{
	CVIBuffer::Free();
}