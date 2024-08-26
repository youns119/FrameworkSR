#include "TriCol.h"

CTriCol::CTriCol()
{
}

CTriCol::CTriCol(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CVIBuffer(_pGraphicDev)
{
}

CTriCol::CTriCol(const CTriCol& _rhs)
	: CVIBuffer(_rhs)
{
}

CTriCol::~CTriCol()
{
}

CTriCol* CTriCol::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CTriCol* pTriCol = new CTriCol(_pGraphicDev);

	if (FAILED(pTriCol->Ready_Buffer()))
	{
		Safe_Release(pTriCol);
		MSG_BOX("TriCol Create Failed");
		return nullptr;
	}

	return pTriCol;
}

HRESULT CTriCol::Ready_Buffer()
{
	m_dwTriCnt = 1;
	m_dwVtxCnt = 3;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_eIdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { 0.f, 1.f, 0.f };
	pVertex[0].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[1].vPosition = { 1.f, -1.f, 0.f };
	pVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[2].vPosition = { -1.f, -1.f, 0.f };
	pVertex[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	m_pIB->Unlock();

	return S_OK;
}

void CTriCol::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CTriCol::Clone()
{
	return new CTriCol(*this);
}

void CTriCol::Free()
{
	CVIBuffer::Free();
}