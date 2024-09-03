#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLight 
	: public CBase
{
private :
	explicit CLight(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CLight();

public :
	static CLight* Create
	(
		LPDIRECT3DDEVICE9 _pGraphicDev,
		const D3DLIGHT9* _pLightInfo,
		const _uint& _iIndex
	);

public :
	HRESULT	Ready_Light(const D3DLIGHT9* _pLightInfo, const _uint& _iIndex);

private :
	virtual void Free();

private :
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	D3DLIGHT9 m_tLightInfo;
	_uint m_iIndex;
};

END