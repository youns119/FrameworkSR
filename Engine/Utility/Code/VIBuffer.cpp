#include "..\..\Header\VIBuffer.h"

CVIBuffer::CVIBuffer()
	: m_pVB(nullptr)
	, m_dwFVF(0)
	, m_dwTriCnt(0)
	, m_dwVtxCnt(0)
	, m_dwVtxSize(0)
{
}

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_dwFVF(0)
	, m_dwTriCnt(0)
	, m_dwVtxCnt(0)
	, m_dwVtxSize(0)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs.m_pGraphicDev)
	, m_pVB(rhs.m_pVB)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxSize(rhs.m_dwVtxSize)
{
	m_pVB->AddRef();
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

	return S_OK;
}

void CVIBuffer::Render_Buffer()
{
}

void CVIBuffer::Free()
{
	CComponent::Free();
}