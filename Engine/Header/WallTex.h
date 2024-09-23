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
	static CWallTex* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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