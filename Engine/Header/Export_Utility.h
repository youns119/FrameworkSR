#pragma once

#include "Management.h"
#include "ComponentManager.h"

#include "TriCol.h"
#include "RcCol.h"

#include "Transform.h"

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

inline void	Release_Utility();

#include "Export_Utility.inl"

END