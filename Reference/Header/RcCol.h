#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol
	: public CVIBuffer
{
private:
	explicit CRcCol();
	explicit CRcCol(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CRcCol(const CRcCol& _rhs);
	virtual ~CRcCol();

public:
	enum BODY_PARTS { HIT_BODY, HIT_HEAD, HIT_CRITICAL, HIT_END };

	static CRcCol* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	_vec3* VertexPos(_int iIndex)
	{
		_vec3 Result = vPos[iIndex];
		return &Result;
	}
	void SetvOffSet(_vec3 pOffSet) { vOffSet = pOffSet; }
	void SetvPos(_int iIndex, _vec3 pPos) { vPos[iIndex] = pPos; }
	void Set_Color(_float r, _float g, _float b, _float a) { TestCol = { r, g , b ,a }; }

	void Set_Hit_Parts(BODY_PARTS _pParts) {
		switch (_pParts)
		{
		case HIT_BODY:
			vPos[0] = { -0.3f, 1.f, 0.4f };
			vPos[1] = { 0.3f, 1.f, 0.4f };
			vPos[2] = { 0.3f, -0.9f, 0.4f };
			vPos[3] = { -0.3f, -0.9f, 0.4f };
			TestCol = { 1.f, 1.f, 0.f, 1.f };
			break;
		case HIT_HEAD:
			vPos[0] = { -0.2f, 1.f, 0.2f };
			vPos[1] = { 0.2f, 1.f, 0.2f };
			vPos[2] = { 0.2f, 0.7f, 0.2f };
			vPos[3] = { -0.2f, 0.7f, 0.2f };
			TestCol = { 1.f, 0.f, 0.f, 1.f };
			break;
		case HIT_CRITICAL:
			vPos[0] = { -0.2f, 0.f, 0.2f };
			vPos[1] = { 0.2f, 0.f, 0.2f };
			vPos[2] = { 0.2f, -0.2f, 0.2f };
			vPos[3] = { -0.2f, -0.2f, 0.2f };
			TestCol = { 0.f, 0.f, 1.f, 1.f };
			break;
		}
	}


public:
	virtual	HRESULT	Ready_Buffer();
	virtual HRESULT OffSet_Buffer();
	virtual	void Render_Buffer();

public:
	CComponent* Clone();

private:
	virtual void Free();
private:
	_vec3 vPos[4];
	_vec3 vOffSet;
	D3DXCOLOR TestCol;
};

END
