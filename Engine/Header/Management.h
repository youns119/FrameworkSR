#pragma once

#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement 
	: public CBase
{
	DECLARE_SINGLETON(CManagement)

private :
	explicit CManagement();
	virtual ~CManagement();

public :
	CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag);

	HRESULT	Set_Scene(CScene* _pScene);

public :
	_int Update_Scene(const _float& _fTimeDelta);
	void LateUpdate_Scene();
	void Render_Scene(LPDIRECT3DDEVICE9 _pGraphicDev);
		
public :
	virtual void Free();

private :
	CScene* m_pScene;
};

END