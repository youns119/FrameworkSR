#pragma once

#include "../Reference/Header/Export_Utility.h"

class CMainScene : public CScene
{
private:
	explicit CMainScene(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMainScene();


public:
	static CMainScene* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_Scene();
	virtual _int Update_Scene(const _float& _fTimeDelta);
	virtual void LateUpdate_Scene();
	virtual void Render_Scene();

private:
	HRESULT Ready_LightInfo();
	HRESULT Ready_Layer_GuideLine(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_ToolCamera(const _tchar* _pLayerTag);


private:
	virtual void Free();


};

