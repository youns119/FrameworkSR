#pragma once

#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene
	: public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CScene();

public:
	const _tchar* Get_Layer(CGameObject* _pGameObject);
	multimap<const _tchar*, CGameObject*>* Get_LayerObjects(const _tchar* _pLayerTag);
	CGameObject* Get_GameObject(const _tchar* _pLayerTag, const _tchar* _pObjTag);
	CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag);

public:
	virtual	HRESULT	Ready_Scene();
	virtual	_int Update_Scene(const _float& _fTimeDelta);
	virtual	void LateUpdate_Scene();
	virtual	void Render_Scene() PURE;

protected:
	virtual	void Free();

protected:
	map<const _tchar*, CLayer*>	m_mapLayer;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
};

END