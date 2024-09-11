#pragma once

#include "Management.h"
#include "ComponentManager.h"
#include "Renderer.h"
#include "LightManager.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "FloorTex.h"
#include "WallTex.h"

#include "Texture.h"

#include "Transform.h"
#include "Camera.h"
#include "Calculator.h"

BEGIN(Engine)

inline HRESULT Create_Management(LPDIRECT3DDEVICE9 _pGraphicDev, CManagement** _ppManagement);

inline CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag);
inline	HRESULT	Set_Scene(CScene* _pScene);
inline	_int Update_Scene(const _float& _fTimeDelta);
inline	void LateUpdate_Scene();
inline	void Render_Scene(LPDIRECT3DDEVICE9 _pGraphicDev);

// ComponentManager
inline HRESULT Ready_Proto(const _tchar* _pComponentTag, CComponent* _pComponent);
inline CComponent* Clone_Proto(const _tchar* _pComponentTag);

// Renderer
inline void	Add_RenderGroup(RENDERID _eType, CGameObject* _pGameObject);
inline void	Render_GameObject(LPDIRECT3DDEVICE9& _pGraphicDev);
inline void	Clear_RenderGroup();

// LightManager
inline HRESULT Ready_Light
(
	LPDIRECT3DDEVICE9 _pGraphicDev,
	const D3DLIGHT9* _pLightInfo,
	const _uint& _iIndex
);

inline void	Release_Utility();

#include "Export_Utility.inl"

END