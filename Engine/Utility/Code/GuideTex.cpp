#include "GuideTex.h"

CGuideTex::CGuideTex()
	:m_pPos(nullptr)
	, m_fY(0)
{
}

CGuideTex::CGuideTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pPos(nullptr)
	, m_fY(0)
{
}

CGuideTex::CGuideTex(const CGuideTex& rhs)
	: CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_fY(0)
{
}

CGuideTex::~CGuideTex()
{

}

HRESULT CGuideTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_eIdxFmt = D3DFMT_INDEX32;

	m_pPos = new _vec3[m_dwVtxCnt];

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = NULL;

	_ulong	dwIndex(0);

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	for (_ulong i = 0; i < dwCntZ; ++i)
	{

		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPosition = _vec3(_float(j) * dwVtxItv, m_fY, _float(i) * dwVtxItv);
			pVertex[dwIndex].vTexUV = _vec2(_float(j) / (dwCntX - 1) * 20.f, _float(i) / (dwCntZ - 1) * 20.f);
			
			m_pPos[dwIndex] = pVertex[dwIndex].vPosition;

		}
	}

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;
	_ulong		dwTriCnt(0);

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;
			dwTriCnt++;

			// 왼쪽 아래
			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;
			dwTriCnt++;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CGuideTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CGuideTex* CGuideTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CGuideTex* pInstance = new CGuideTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		MSG_BOX("GuideTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CGuideTex::Clone()
{
	return new CGuideTex(*this);
}

void CGuideTex::Free()
{
	if (m_bClone == false)
		Safe_Delete_Array(m_pPos);
	CVIBuffer::Free();
}
