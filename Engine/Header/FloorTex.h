#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CFloorTex : public CVIBuffer
{
private:
	explicit CFloorTex();
	explicit CFloorTex(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CFloorTex(const CFloorTex& _rhs);
	virtual ~CFloorTex();

public:
	const _vec3* Get_VtxPos() const { return m_pPos; }

public:
	static CFloorTex* Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _ulong& _dwCntX = VTXCNTX, const _ulong& _dwCntZ = VTXCNTZ, const _ulong& _dwVtxItv = VTXITV);

public:
	HRESULT	Ready_Buffer(const _ulong& _dwCntX, const _ulong& _dwCntZ, const _ulong& _dwVtxItv);
	virtual	void Render_Buffer();

public:
	virtual	CComponent* Clone();

private:
	virtual void Free();

private:
	/*BITMAPFILEHEADER m_tFH;
	BITMAPINFOHEADER m_tIH;

	HANDLE m_hFile;*/

	_vec3* m_pPos;
};

END
