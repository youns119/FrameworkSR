#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CLaserTex :
	public CVIBuffer
{
private:
	explicit CLaserTex();
	explicit CLaserTex(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CLaserTex(const CLaserTex& _rhs);
	virtual ~CLaserTex();

public:
	static CLaserTex* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_Buffer();
	virtual void Render_Buffer();

public:
	virtual	CComponent* Clone();

private:
	virtual void Free();

public:
	void Set_UV(const _vec2& _vGrid, const _uint& iIndex);
	_vec2 Get_UV(_uint _iIndex = 0)
	{
		VTXTEX* pVertex;
		_vec2 vResult;
		m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		vResult = pVertex[_iIndex].vTexUV;

		m_pVB->Unlock();

		return vResult;
	}
};

END