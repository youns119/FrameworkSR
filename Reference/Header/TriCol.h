#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol 
	: public CVIBuffer
{
private :
	explicit CTriCol();
	explicit CTriCol(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CTriCol(const CTriCol& _rhs);
	virtual ~CTriCol();

public :
	static CTriCol* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	virtual	HRESULT	Ready_Buffer();
	virtual	void Render_Buffer();

public :
	CComponent* Clone();

private :
	virtual void Free();
};

END