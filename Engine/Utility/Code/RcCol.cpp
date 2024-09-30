#include "RcCol.h"

CRcCol::CRcCol()
{
}

CRcCol::CRcCol(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CVIBuffer(_pGraphicDev)
{
	vPos[0] = { -0.5f, 1.f, 0.f };
	vPos[1] = { 0.5f, 1.f, 0.f };
	vPos[2] = { 0.5f, -1.f, 0.f };
	vPos[3] = { -0.5f, -1.f, 0.f };
}

CRcCol::CRcCol(const CRcCol& _rhs)
	: CVIBuffer(_rhs)
{
	vPos[0] = _rhs.vPos[0];
	vPos[1] = _rhs.vPos[1];
	vPos[2] = _rhs.vPos[2];
	vPos[3] = _rhs.vPos[3];
}

CRcCol::~CRcCol()
{
}

CRcCol* CRcCol::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CRcCol* pRcCol = new CRcCol(_pGraphicDev);

	if (FAILED(pRcCol->Ready_Buffer()))
	{
		Safe_Release(pRcCol);
		MSG_BOX("RcCol Create Failed");
		return nullptr;
	}

	return pRcCol;
}

HRESULT CRcCol::Ready_Buffer()
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_eIdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = vPos[0];
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPosition = vPos[1];
	pVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[2].vPosition = vPos[2];
	pVertex[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[3].vPosition = vPos[3];
	pVertex[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CRcCol::OffSet_Buffer()
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_eIdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = vPos[0] + vOffSet;
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPosition = vPos[1] + vOffSet;
	pVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[2].vPosition = vPos[2] + vOffSet;
	pVertex[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[3].vPosition = vPos[3] + vOffSet;
	pVertex[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}




void CRcCol::Render_Buffer()
{
	OffSet_Buffer();
	CVIBuffer::Render_Buffer();
}

CComponent* CRcCol::Clone()
{
	return new CRcCol(*this);
}

void CRcCol::Free()
{
	CVIBuffer::Free();
}