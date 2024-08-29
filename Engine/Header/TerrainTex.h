#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex 
	: public CVIBuffer
{
private :
	explicit CTerrainTex();
	explicit CTerrainTex(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& _rhs);
	virtual ~CTerrainTex();

public :
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _ulong& _dwCntX = VTXCNTX, const _ulong& _dwCntZ = VTXCNTZ, const _ulong& _dwVtxItv = VTXITV);

public :
	HRESULT	Ready_Buffer(const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv);
	virtual	void Render_Buffer();

public :
	virtual	CComponent* Clone();

private :
	virtual void Free();
};

END