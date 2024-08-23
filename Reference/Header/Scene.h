#pragma once

#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene 
	: public CBase
{
protected :
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public :
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag);

public :
	virtual	HRESULT	Ready_Scene();
	virtual	_int Update_Scene(const _float& fTimeDelta);
	virtual	void LateUpdate_Scene();
	virtual	void Render_Scene();

protected :
	virtual	void Free();

protected :
	map<const _tchar*, CLayer*>	m_mapLayer;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
};

END