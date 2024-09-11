#include "WallTex.h"

CWallTex::CWallTex()
	: m_pPos(nullptr)
{
}

CWallTex::CWallTex(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CVIBuffer(_pGraphicDev)
	, m_pPos(nullptr)
{
}

CWallTex::CWallTex(const CWallTex& _rhs)
	:CVIBuffer(_rhs)
	, m_pPos(_rhs.m_pPos)
{
}

CWallTex::~CWallTex()
{
}

CWallTex* CWallTex::Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _ulong& _dwCntX, const _ulong& _dwCntY, const _ulong& _dwCntZ, const _ulong& _dwVtxItv)
{
	CWallTex* pInstance = new CWallTex(_pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(_dwCntX, _dwCntY, _dwCntZ, _dwVtxItv)))
	{
		Safe_Release(pInstance);
		MSG_BOX("WallTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CWallTex::Ready_Buffer(const _ulong& _dwCntX, const _ulong& _dwCntY, const _ulong& _dwCntZ, const _ulong& _dwVtxItv)
{

	m_dwVtxCnt = _dwCntY * _dwCntZ;
	m_dwTriCnt = (_dwCntX - 1) * (_dwCntZ - 1) * 2;
	
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
		for (_ulong j = 0; j < _dwCntY - 1; ++j)
		{
			dwIndex = i * _dwCntY + j;

			pVertex[dwIndex].vPosition = _vec3(0.f, _float(j) * _dwVtxItv, _float(i) * _dwVtxItv);
			pVertex[dwIndex].vTexUV = _vec2(_float(j) / (_dwCntY - 1) ,
				_float(i) / (_dwCntZ - 1) );
		}
	}

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;
	_ulong	 dwTriCnt(0);

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < _dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < _dwCntY - 1; ++j)
		{
			dwIndex = i * _dwCntY + j;

	//		//오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + _dwCntY;
			pIndex[dwTriCnt]._1 = dwIndex + _dwCntY + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;
			dwTriCnt++;

	//		//왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + _dwCntY;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
			dwTriCnt++;

		}
	}

	m_pIB->Unlock();


	return S_OK;
}

void CWallTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CWallTex::Clone()
{
	return new CWallTex(*this);

}

void CWallTex::Free()
{
}
