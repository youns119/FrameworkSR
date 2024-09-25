#pragma once
#include "Scene.h"
#include "Export_Utility.h"
#include "Export_System.h"

class CMapCreate : public CScene
{
private:
	explicit CMapCreate(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMapCreate();

public:
	static CMapCreate* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_Scene();
	virtual _int Update_Scene(const _float& _fTimeDelta);
	virtual void LateUpdate_Scene();
	virtual void Render_Scene();

private:
	HRESULT Ready_LightInfo();
	HRESULT Ready_Layer_ToolEnvironment(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* _pLayerTag);


	HRESULT Ready_Layer_PickingTile(const _tchar* _pLayerTag);
	CLayer* Find_Layer_PickingTile();
	HRESULT Create_Layer_PickingTile(CLayer* _pLayer);


	_vec3 TilePiking_OnTerrain();
	_vec3 TileCreate_OnTerrain(HWND _hWnd, CGuideTex* _pGuideBufferCom);

	//imgui ÇÔ¼ö
	void ShowGUI();
	void SetMenu();

private:
	virtual void Free();
};

