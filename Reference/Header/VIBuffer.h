#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer 
	: public CComponent
{
protected :
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& _rhs);
	virtual ~CVIBuffer();

public :
	virtual	HRESULT	Ready_Buffer();
	virtual	void Render_Buffer();

protected :
	virtual void Free();

protected :
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	_ulong m_dwVtxCnt;
	_ulong m_dwVtxSize;
	_ulong m_dwTriCnt;
	_ulong m_dwFVF;

	_ulong m_dwIdxSize;
	D3DFORMAT m_eIdxFmt;
};

END