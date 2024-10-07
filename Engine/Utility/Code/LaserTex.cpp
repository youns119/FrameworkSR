#include"../../Header/LaserTex.h"

CLaserTex::CLaserTex()
{
}

CLaserTex::CLaserTex(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CVIBuffer(_pGraphicDev)
{
}

CLaserTex::CLaserTex(const CLaserTex& _rhs)
	: CVIBuffer(_rhs)
{
}

CLaserTex::~CLaserTex()
{
}

CLaserTex* CLaserTex::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CLaserTex* pInstance = new CLaserTex(_pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("LaserTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CLaserTex::Ready_Buffer()
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_eIdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = { -1.f, -1.f, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };

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

void CLaserTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CLaserTex::Clone()
{
	return new CLaserTex(*this);
}

void CLaserTex::Free()
{
	CVIBuffer::Free();
}

void CLaserTex::Set_UV(const _vec2& _vGrid, const _uint& iIndex)
{
	_uint iRow, iCol;
	iRow = iIndex / (_uint)_vGrid.x;
	iCol = iIndex % (_uint)_vGrid.x;

	_vec2 vStride;
	vStride.x = 1.f / _vGrid.x;
	vStride.y = 1.f / _vGrid.y;

	_vec2 vNewUV = { iCol / _vGrid.x, iRow / _vGrid.y };

	VTXTEX* pVertex(nullptr);
	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
	{
		MSG_BOX("m_pVB Lock Failed");
		return;
	}

	pVertex[0].vTexUV = vNewUV;
	pVertex[1].vTexUV = vNewUV + _vec2(vStride.x, 0.f);
	pVertex[2].vTexUV = vNewUV + vStride;
	pVertex[3].vTexUV = vNewUV + _vec2(0.f, vStride.y);

	m_pVB->Unlock();
}