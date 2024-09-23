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
	static CFloorTex* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
