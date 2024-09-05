#pragma once

#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;

class ENGINE_DLL CCalculator 
	: public CComponent
{
private :
	explicit CCalculator(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CCalculator();

public :
	static CCalculator* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	HRESULT	Ready_Calculator();
	_float Compute_HeightOnTerrain
	(
		const _vec3* _pPos,
		const _vec3* _pTerrainVtxPos,
		const _ulong& _dwCntX,
		const _ulong& _dwCntZ,
		const _ulong& _dwVtxItv = VTXITV
	);
	_vec3 Picking_OnTerrain
	(
		HWND _hWnd, 
		CTerrainTex* _pTerrainBufferCom,
		CTransform* _pTerrainTransCom
	);

public :
	virtual CCalculator* Clone();

private :
	virtual void Free();
};

END