#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol 
	: public CVIBuffer
{
private :
	explicit CRcCol();
	explicit CRcCol(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CRcCol(const CRcCol& _rhs);
	virtual ~CRcCol();

public :
	static CRcCol* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	_vec3* VertexPos(_int iIndex)
	{
		_vec3 Result = vPos[iIndex] + vOffSet;
		return &Result;
	}
	void SetvOffSet(_vec3 pOffSet) { vOffSet = pOffSet; }
public :
	virtual	HRESULT	Ready_Buffer();
	virtual HRESULT OffSet_Buffer();
	virtual	void Render_Buffer();

public :
	CComponent* Clone();

private :
	virtual void Free();
private:
	_vec3 vPos[4];
	_vec3 vOffSet;
};

END