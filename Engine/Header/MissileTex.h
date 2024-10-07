#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMissileTex : public CVIBuffer
{
private:
	explicit CMissileTex();
	explicit CMissileTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMissileTex(const CMissileTex& rhs);
	virtual ~CMissileTex();

public:
	static CMissileTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual	HRESULT	Ready_Buffer();
	virtual	void Render_Buffer();

public:
	virtual	CComponent* Clone();

private:
	virtual void Free();

};

END