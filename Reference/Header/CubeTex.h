#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex 
	: public CVIBuffer
{
private :
	explicit CCubeTex();
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();

public :
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public :
	virtual	HRESULT	Ready_Buffer();
	virtual	void Render_Buffer();

public :
	virtual	CComponent* Clone();

private :
	virtual void Free();
};

END