#include "..\..\Header\Calculator.h"
#include "Export_Utility.h"

CCalculator::CCalculator(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
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
	_ulong dwIndex = _ulong(_pPos->z / _dwVtxItv) * _dwCntX + _ulong(_pPos->x / _dwVtxItv);
	if (_dwCntX * _dwCntZ <= dwIndex)
		return 0;

	_float fRatioX = (_pPos->x - _pTerrainVtxPos[dwIndex + _dwCntX].x) / _dwVtxItv;
	_float fRatioZ = (_pTerrainVtxPos[dwIndex + _dwCntX].z - _pPos->z) / _dwVtxItv;

	D3DXPLANE Plane;

	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints
		(
			&Plane,
			&_pTerrainVtxPos[dwIndex + _dwCntX],
			&_pTerrainVtxPos[dwIndex + _dwCntX + 1],
			&_pTerrainVtxPos[dwIndex + 1]
		);
	}
	else
	{
		D3DXPlaneFromPoints
		(
			&Plane,
			&_pTerrainVtxPos[dwIndex + _dwCntX],
			&_pTerrainVtxPos[dwIndex + 1],
			&_pTerrainVtxPos[dwIndex]
		);
	}

	return (-Plane.a * _pPos->x - Plane.c * _pPos->z - Plane.d) / Plane.b;
}

_vec3 CCalculator::Picking_OnTerrain
(
	HWND _hWnd,
	CTerrainTex* _pTerrainBufferCom,
	CTransform* _pTerrainTransCom
)
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(_hWnd, &ptMouse);

	_vec3 vMousePos;
	D3DVIEWPORT9 ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 1.f;

	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3 vRayPos, vRayDir;
	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix	matWorld;
	_pTerrainTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	const _vec3* pTerrainTexPos = _pTerrainBufferCom->Get_VtxPos();

	_ulong	dwVtxId[3]{};
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < VTXCNTZ - 1; i++)
	{
		for (_ulong j = 0; j < VTXCNTX - 1; j++)
		{
			_ulong	dwIndex = i * VTXCNTX + j;

			dwVtxId[0] = dwIndex + VTXCNTX;
			dwVtxId[1] = dwIndex + VTXCNTX + 1;
			dwVtxId[2] = dwIndex + 1;

			if (D3DXIntersectTri
			(
				&pTerrainTexPos[dwVtxId[1]],
				&pTerrainTexPos[dwVtxId[2]],
				&pTerrainTexPos[dwVtxId[0]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist
			))
			{
				return _vec3
				(
					pTerrainTexPos[dwVtxId[1]].x + fU * (pTerrainTexPos[dwVtxId[2]].x - pTerrainTexPos[dwVtxId[1]].x),
					0.f,
					pTerrainTexPos[dwVtxId[1]].z + fV * (pTerrainTexPos[dwVtxId[0]].z - pTerrainTexPos[dwVtxId[1]].z)
				);
			}

			dwVtxId[0] = dwIndex + VTXCNTX;
			dwVtxId[1] = dwIndex + 1;
			dwVtxId[2] = dwIndex;

			if (D3DXIntersectTri
			(
				&pTerrainTexPos[dwVtxId[2]],
				&pTerrainTexPos[dwVtxId[0]],
				&pTerrainTexPos[dwVtxId[1]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist
			))
			{
				// V1 + U(V2 - V1) + V(V3 - V1)

				return _vec3
				(
					pTerrainTexPos[dwVtxId[2]].x + fU * (pTerrainTexPos[dwVtxId[0]].x - pTerrainTexPos[dwVtxId[2]].x),
					0.f,
					pTerrainTexPos[dwVtxId[2]].z + fV * (pTerrainTexPos[dwVtxId[1]].z - pTerrainTexPos[dwVtxId[2]].z)
				);
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

CCalculator* CCalculator::Clone()
{
	return new CCalculator(*this);
}

void CCalculator::Free()
{
	CComponent::Free();
}