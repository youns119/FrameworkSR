#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol 
	: public CVIBuffer
{
private :
	explicit CRcCol();
	explicit CRcCol(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CRcCol(const CRcCol& _rhs);
	virtual ~CRcCol();

public :
	static CRcCol* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	virtual	HRESULT	Ready_Buffer();
	virtual	void Render_Buffer();

public :
	CComponent* Clone();

private :
	virtual void Free();
};

END