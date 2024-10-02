#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CGuideTex : public CVIBuffer
{

private:
	explicit CGuideTex();
	explicit CGuideTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGuideTex(const CGuideTex& rhs);
	virtual ~CGuideTex();
public:
	const _vec3* Get_VtxPos() const { return m_pPos; }
	void Set_ChangeY(float _fY) { m_fY = _fY; }

public:
	HRESULT		Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual		void		Render_Buffer();

public:
	static		CGuideTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX = VTXCNTX, const _ulong& dwCntZ = VTXCNTZ, const _ulong& dwVtxItv = VTXITV);
	virtual		CComponent* Clone();

private:
	virtual void		Free();
	_vec3* m_pPos;
	float m_fY;
};

END