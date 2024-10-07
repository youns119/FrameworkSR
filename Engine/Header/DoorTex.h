#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CDoorTex : public CVIBuffer
{
private:
	explicit CDoorTex();
	explicit CDoorTex(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CDoorTex(const CDoorTex& _rhs);
	virtual ~CDoorTex();

public:
	const _vec3* Get_VtxPos() const { return m_pPos; }

public:
	static CDoorTex* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT	Ready_Buffer();
	virtual	void Render_Buffer();

public:
	virtual	CComponent* Clone();

private:
	virtual void Free();

private:
	_vec3* m_pPos;
};

END