#include "FloorTex.h"

CFloorTex::CFloorTex()
	: m_pPos(nullptr)
{
}

CFloorTex::CFloorTex(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CVIBuffer(_pGraphicDev)
	, m_pPos(nullptr)
{
}

CFloorTex::CFloorTex(const CFloorTex& _rhs)
	:CVIBuffer(_rhs)
	, m_pPos(_rhs.m_pPos)
{
}

CFloorTex::~CFloorTex()
{
}

CFloorTex* CFloorTex::Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv)
{
	CFloorTex* pInstance = new CFloorTex(_pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(_dwCntX, _dwCntZ, _dwVtxItv)))
	{
		Safe_Release(pInstance);
		MSG_BOX("FloorTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CFloorTex::Ready_Buffer(const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv)
{
	m_dwVtxCnt = _dwCntX * _dwCntZ; // Vertex Count // 512 * 512
	m_dwTriCnt = (_dwCntX - 1) * (_dwCntZ - 1) * 2; // Triangle count
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_eIdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = NULL;

	_ulong dwIndex(0);

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < _dwCntZ; ++i)
	{
		if (i == 130)
			continue;

		for (_ulong j = 0; j < _dwCntX - 1; ++j)
		{
			dwIndex = i * _dwCntX + j;

			pVertex[dwIndex].vPosition = _vec3(_float(j) * _dwVtxItv, 0.f, _float(i) * _dwVtxItv);
			pVertex[dwIndex].vTexUV = _vec2(_float(j) / (_dwCntX - 1) * 20.f, 
									  _float(i) / (_dwCntZ - 1) * 20.f);
		}
	}

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;
	_ulong	 dwTriCnt(0);

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < _dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < _dwCntX - 1; ++j)
		{
			dwIndex = i * _dwCntX + j;

			//오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + _dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + _dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;
			dwTriCnt++;

			//왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + _dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
			dwTriCnt++;

		}
	}

	m_pIB->Unlock();


	return S_OK;
}

void CFloorTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CFloorTex::Clone()
{
	return new CFloorTex(*this);
}

void CFloorTex::Free()
{
}
