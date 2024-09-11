#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CWallTex : public CVIBuffer
{
private:
	explicit CWallTex();
	explicit CWallTex(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CWallTex(const CWallTex& _rhs);
	virtual ~CWallTex();

public:
	const _vec3* Get_VtxPos() const { return m_pPos; }

public:
	static CWallTex* Create(LPDIRECT3DDEVICE9 _pGraphicDev, 
		const _ulong& _dwCntX,
		const _ulong& _dwCntY,
		const _ulong& _dwCntZ,
		const _ulong& _dwVtxItv);

public:
	HRESULT	Ready_Buffer(const _ulong& _dwCntX, const _ulong& _dwCntY, const _ulong& _dwCntZ, const _ulong& _dwVtxItv);
	virtual	void Render_Buffer();

public:
	virtual	CComponent* Clone();

private:
	virtual void Free();

private:
	_vec3* m_pPos;
};

END