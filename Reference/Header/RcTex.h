#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex 
	: public CVIBuffer
{
private :
	explicit CRcTex();
	explicit CRcTex(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CRcTex(const CRcTex& _rhs);
	virtual ~CRcTex();

public :
	static CRcTex* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	virtual HRESULT	Ready_Buffer();
	virtual void Render_Buffer();

public :
	virtual	CComponent* Clone();

private :
	virtual void Free();
};

END