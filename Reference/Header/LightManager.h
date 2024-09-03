#pragma once

#include "Light.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLightManager 
	: public CBase
{
	DECLARE_SINGLETON(CLightManager)

private :
	explicit CLightManager();
	virtual ~CLightManager();

public :
	HRESULT	Ready_Light
	(
		LPDIRECT3DDEVICE9 _pGraphicDev,
		const D3DLIGHT9* _pLightInfo,
		const _uint& _iIndex
	);

private :
	virtual void Free();

private :
	list<CLight*> m_LightList;
};

END