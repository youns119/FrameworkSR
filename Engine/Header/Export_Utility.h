#pragma once

#include "Management.h"
#include "ComponentManager.h"
#include "Renderer.h"
#include "LightManager.h"
// 연욱
#include "UIManager.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "GuideTex.h"
#include "CubeTex.h"
#include "FloorTex.h"
#include "WallTex.h"

#include "Texture.h"

#include "Transform.h"
#include "Camera.h"
#include "Calculator.h"
#include "ComponentCamera.h"
// 연욱
#include "Collider.h"
#include "CollisionManager.h"
#include "Animator.h"
#include "Animation.h"

// kyubin
#include "Effect.h"
#include "ParticleSystem.h"
#include "ParticleSpark.h"

//Jonghan Change
#include "BulletManager.h"

BEGIN(Engine)

inline HRESULT Create_Management(LPDIRECT3DDEVICE9 _pGraphicDev, CManagement** _ppManagement);

inline CScene* Get_CurrScene();
inline CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag);
inline CONTROLLERID Get_ControllerID();
inline	HRESULT	Set_Scene(CScene* _pScene);
inline void Set_ControllerID(CONTROLLERID _eController);
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

// 연욱
// UIManager
inline HRESULT Add_UI(CUI* _pUI);
inline _int Update_UI(const _float& _fTimeDelta);
inline void LateUpdate_UI();
inline void Render_UI(LPDIRECT3DDEVICE9& _pGraphicDev);
inline list<CUI*>* Get_UIList(UITYPE _eUIType);

// CollisionManager
inline _bool Get_ColliderRender();
inline void Toggle_Collider();
inline void Update_Collision();
inline void Render_Collider();
inline void Add_Collider(CCollider* _pCollider);
inline void CheckGroup(const _tchar* _pLeft, const _tchar* _pRight);
inline void Clear_Collider();
inline void Reset();

//Jonghan Change
inline HRESULT Add_Bullet(CBullet* _pBullet);
inline HRESULT Fire_Bullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage, CBulletManager::BULLETTYPE _eBulletType);
inline _int Update_Bullet(const _float& _fTimeDelta);
inline void LateUpdate_Bullet();

inline void	Release_Utility();

#include "Export_Utility.inl"

END