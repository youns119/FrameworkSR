#include "..\..\Header\VIBuffer.h"

CVIBuffer::CVIBuffer()
	: m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwFVF(0)
	, m_dwTriCnt(0)
	, m_dwVtxCnt(0)
	, m_dwVtxSize(0)
	, m_dwIdxSize(0)
	, m_eIdxFmt(D3DFMT_INDEX32)
{
}

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwFVF(0)
	, m_dwTriCnt(0)
	, m_dwVtxCnt(0)
	, m_dwVtxSize(0)
	, m_dwIdxSize(0)
	, m_eIdxFmt(D3DFMT_INDEX32)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& _rhs)
	: CComponent(_rhs)
	, m_pVB(_rhs.m_pVB)
	, m_pIB(_rhs.m_pIB)
	, m_dwFVF(_rhs.m_dwFVF)
	, m_dwTriCnt(_rhs.m_dwTriCnt)
	, m_dwVtxCnt(_rhs.m_dwVtxCnt)
	, m_dwVtxSize(_rhs.m_dwVtxSize)
	, m_dwIdxSize(_rhs.m_dwIdxSize)
	, m_eIdxFmt(_rhs.m_eIdxFmt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready_Buffer()
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer
	(
		m_dwVtxCnt * m_dwVtxSize,
		0,
		m_dwFVF,
		D3DPOOL_MANAGED,
		&m_pVB, 
		NULL
	), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer
	(
		m_dwTriCnt * m_dwIdxSize,
		0,
		m_eIdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB, 
		NULL
	), E_FAIL);

	return S_OK;
}

void CVIBuffer::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF);

	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CVIBuffer::Free()
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CComponent::Free();
}