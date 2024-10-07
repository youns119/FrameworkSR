#include "../../Header/MissileTex.h"

CMissileTex::CMissileTex()
{
}

CMissileTex::CMissileTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CMissileTex::CMissileTex(const CMissileTex& rhs)
	: CVIBuffer(rhs)
{
}

CMissileTex::~CMissileTex()
{
}

CMissileTex* CMissileTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissileTex* pInstance = new CMissileTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CubeTex Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CMissileTex::Ready_Buffer()
{
	m_dwTriCnt = 36;
	m_dwVtxCnt = 24;
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwFVF = FVF_CUBE;

	m_dwIdxSize = sizeof(INDEX32);
	m_eIdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCUBE* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -1.f, 1.f, -2.f };
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = { 1.f, 1.f, -2.f };
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = { 1.f, -1.f, -2.f };
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = { -1.f, -1.f, -2.f };
	pVertex[3].vTexUV = pVertex[3].vPosition;

	pVertex[4].vPosition = { -1.f, 1.f, 1.f };
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = { 1.f, 1.f, 1.f };
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = { 1.f, -1.f, 1.f };
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = { -1.f, -1.f, 1.f };
	pVertex[7].vTexUV = pVertex[7].vPosition;
	//--------------------------//
	pVertex[8].vPosition = { 1.f, 0.5f, 0.f };
	pVertex[8].vTexUV = pVertex[8].vPosition;

	pVertex[9].vPosition = { 1.5f, 0.5f, 0.f };
	pVertex[9].vTexUV = pVertex[9].vPosition;

	pVertex[10].vPosition = { 1.5f, -0.5f, 0.f };
	pVertex[10].vTexUV = pVertex[10].vPosition;

	pVertex[11].vPosition = { 1.f, -0.5f, 0.f };
	pVertex[11].vTexUV = pVertex[11].vPosition;

	pVertex[12].vPosition = { 1.f, 0.5f, 1.5f };
	pVertex[12].vTexUV = pVertex[12].vPosition;

	pVertex[13].vPosition = { 1.5f, 0.5f, 1.5f };
	pVertex[13].vTexUV = pVertex[13].vPosition;

	pVertex[14].vPosition = { 1.5f, -0.5f, 1.5f };
	pVertex[14].vTexUV = pVertex[14].vPosition;

	pVertex[15].vPosition = { 1.f, -0.5f, 1.5f };
	pVertex[15].vTexUV = pVertex[15].vPosition;
	/////////////
	pVertex[16].vPosition = { -1.5f, 0.5f, 0.f };
	pVertex[16].vTexUV = pVertex[16].vPosition;

	pVertex[17].vPosition = { -1.f, 0.5f, 0.f };
	pVertex[17].vTexUV = pVertex[17].vPosition;

	pVertex[18].vPosition = { -1.f, -0.5f, 0.f };
	pVertex[18].vTexUV = pVertex[18].vPosition;

	pVertex[19].vPosition = { -1.5f, -0.5f, 0.f };
	pVertex[19].vTexUV = pVertex[19].vPosition;

	pVertex[20].vPosition = { -1.5f, 0.5f, 1.5f };
	pVertex[20].vTexUV = pVertex[20].vPosition;

	pVertex[21].vPosition = { -1.f, 0.5f, 1.5f };
	pVertex[21].vTexUV = pVertex[21].vPosition;

	pVertex[22].vPosition = { -1.f, -0.5f, 1.5f };
	pVertex[22].vTexUV = pVertex[22].vPosition;

	pVertex[23].vPosition = { -1.5f, -0.5f, 1.5f };
	pVertex[23].vTexUV = pVertex[23].vPosition;

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;
	////////////////////////
	pIndex[12]._0 = 9;
	pIndex[12]._1 = 13;
	pIndex[12]._2 = 14;

	pIndex[13]._0 = 9;
	pIndex[13]._1 = 14;
	pIndex[13]._2 = 10;

	pIndex[14]._0 = 12;
	pIndex[14]._1 = 8;
	pIndex[14]._2 = 11;

	pIndex[15]._0 = 8;
	pIndex[15]._1 = 11;
	pIndex[15]._2 = 15;

	pIndex[16]._0 = 12;
	pIndex[16]._1 = 13;
	pIndex[16]._2 = 9;

	pIndex[17]._0 = 12;
	pIndex[17]._1 = 9;
	pIndex[17]._2 = 8;

	pIndex[18]._0 = 11;
	pIndex[18]._1 = 10;
	pIndex[18]._2 = 14;

	pIndex[19]._0 = 11;
	pIndex[19]._1 = 14;
	pIndex[19]._2 = 15;

	pIndex[20]._0 = 15;
	pIndex[20]._1 = 14;
	pIndex[20]._2 = 13;

	pIndex[21]._0 = 15;
	pIndex[21]._1 = 13;
	pIndex[21]._2 = 12;

	pIndex[22]._0 = 8;
	pIndex[22]._1 = 9;
	pIndex[22]._2 = 10;

	pIndex[23]._0 = 8;
	pIndex[23]._1 = 10;
	pIndex[23]._2 = 11;

	/////////////////

	pIndex[24]._0 = 17;
	pIndex[24]._1 = 21;
	pIndex[24]._2 = 22;

	pIndex[25]._0 = 17;
	pIndex[25]._1 = 22;
	pIndex[25]._2 = 18;

	pIndex[26]._0 = 20;
	pIndex[26]._1 = 16;
	pIndex[26]._2 = 19;

	pIndex[27]._0 = 20;
	pIndex[27]._1 = 19;
	pIndex[27]._2 = 23;

	pIndex[28]._0 = 20;
	pIndex[28]._1 = 21;
	pIndex[28]._2 = 17;

	pIndex[29]._0 = 20;
	pIndex[29]._1 = 17;
	pIndex[29]._2 = 16;

	pIndex[30]._0 = 19;
	pIndex[30]._1 = 18;
	pIndex[30]._2 = 22;

	pIndex[31]._0 = 19;
	pIndex[31]._1 = 22;
	pIndex[31]._2 = 23;

	pIndex[32]._0 = 23;
	pIndex[32]._1 = 22;
	pIndex[32]._2 = 21;

	pIndex[33]._0 = 23;
	pIndex[33]._1 = 21;
	pIndex[33]._2 = 20;

	pIndex[34]._0 = 16;
	pIndex[34]._1 = 17;
	pIndex[34]._2 = 18;

	pIndex[35]._0 = 16;
	pIndex[35]._1 = 18;
	pIndex[35]._2 = 19;

	m_pIB->Unlock();

	return S_OK;
}

void CMissileTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CMissileTex::Clone()
{
	return new CMissileTex(*this);
}

void CMissileTex::Free()
{
	CVIBuffer::Free();
}