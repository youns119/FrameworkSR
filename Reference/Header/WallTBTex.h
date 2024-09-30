#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CWallTBTex : public CVIBuffer
{
private:
	explicit CWallTBTex();
	explicit CWallTBTex(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CWallTBTex(const CWallTBTex& _rhs);
	virtual ~CWallTBTex();

public:
	const _vec3* Get_VtxPos() const { return m_pPos; }

public:
	static CWallTBTex* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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