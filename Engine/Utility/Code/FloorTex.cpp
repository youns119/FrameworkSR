#include "FloorTex.h"

CFloorTex::CFloorTex()
{
	for (int i = 0; i < 4; i++)
		m_vPos[i] = { 0.f, 0.f, 0.f };
}

CFloorTex::CFloorTex(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CVIBuffer(_pGraphicDev)
{
	for (int i = 0; i < 4; i++)
		m_vPos[i] = { 0.f, 0.f, 0.f };
}

CFloorTex::CFloorTex(const CFloorTex& _rhs)
	:CVIBuffer(_rhs)
{
	for (int i = 0; i < 4; i++)
		m_vPos[i] = { _rhs.m_vPos[i].x, _rhs.m_vPos[i].y, _rhs.m_vPos[i].z };
}

CFloorTex::~CFloorTex()
{
}

CFloorTex* CFloorTex::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CFloorTex* pInstance = new CFloorTex(_pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))



	{
		Safe_Release(pInstance);
		MSG_BOX("FloorTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CFloorTex::Ready_Buffer()
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

	m_vPos[0] = pVertex[0].vPosition = { 0.f, 0.f, 1.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	m_vPos[1] = pVertex[1].vPosition = { 1.f, 0.f, 1.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	m_vPos[2] = pVertex[2].vPosition = { 1.f, 0.f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	m_vPos[3] = pVertex[3].vPosition = { 0.f, 0.f, 0.f };
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
	CVIBuffer::Free();
}
