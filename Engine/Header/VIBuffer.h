#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer 
	: public CComponent
{
protected :
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public :
	virtual	HRESULT	Ready_Buffer();
	virtual	void Render_Buffer();

public :
	virtual void Free();

protected :
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	_ulong m_dwVtxCnt;
	_ulong m_dwVtxSize;
	_ulong m_dwTriCnt;
	_ulong m_dwFVF;
};

END