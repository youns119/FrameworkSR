#include "..\..\Header\Calculator.h"

CCalculator::CCalculator(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CComponent(_pGraphicDev)
{
}

CCalculator::~CCalculator()
{
}

CCalculator* CCalculator::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CCalculator* pCalculator = new CCalculator(_pGraphicDev);

	if (FAILED(pCalculator->Ready_Calculator()))
	{
		Safe_Release(pCalculator);
		return nullptr;
	}

	return pCalculator;
}

HRESULT CCalculator::Ready_Calculator()
{
	return S_OK;
}

_float CCalculator::Compute_HeightOnTerrain
(
	const _vec3* _pPos, 
	const _vec3* _pTerrainVtxPos, 
	const _ulong& _dwCntX, 
	const _ulong& _dwCntZ, 
	const _ulong& _dwVtxItv
)
{
	// 특정 점의 위치 (x,y,z)
	// 평면이 지닌 법선 벡터(a, b, c, d)

	_ulong	dwIndex = _ulong(_pPos->z / _dwVtxItv) * _dwCntX + _ulong(_pPos->x / _dwVtxItv);

	_float	fRatioX = (_pPos->x - _pTerrainVtxPos[dwIndex + _dwCntX].x) / _dwVtxItv;
	_float	fRatioZ = (_pTerrainVtxPos[dwIndex + _dwCntX].z - _pPos->z) / _dwVtxItv;

	D3DXPLANE	Plane;

	// 우상단
	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane,
			&_pTerrainVtxPos[dwIndex + _dwCntX],
			&_pTerrainVtxPos[dwIndex + _dwCntX + 1],
			&_pTerrainVtxPos[dwIndex + 1]);
	}

	else
	{
		D3DXPlaneFromPoints(&Plane,
			&_pTerrainVtxPos[dwIndex + _dwCntX],
			&_pTerrainVtxPos[dwIndex + 1],
			&_pTerrainVtxPos[dwIndex]);
	}

	// ax + by + cz + d
	// 
	// -by = ax + cz + d 

	//y = (-ax - cz - d) / b

	return (-Plane.a * _pPos->x - Plane.c * _pPos->z - Plane.d) / Plane.b;
}

CCalculator* CCalculator::Clone()
{
	return new CCalculator(*this);
}

void CCalculator::Free()
{
	CComponent::Free();
}