#pragma once

#include "Management.h"
#include "ComponentManager.h"
#include "Renderer.h"
#include "LightManager.h"

// 연욱
#include "UIUnit.h"
#include "UI.h"
#include "UIManager.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "GuideTex.h"
#include "LaserTex.h"
#include "MissileTex.h"
#include "CubeTex.h"
#include "FloorTex.h"
#include "WallTex.h"

#include "WallTBTex.h"

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
#include "Character.h"
#include "Trigger.h"
#include "DrinkSpawner.h"

BEGIN(Engine)

inline HRESULT Create_Management(LPDIRECT3DDEVICE9 _pGraphicDev, CManagement** _ppManagement);

inline CScene* Get_CurrScene();
inline CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag);
inline CONTROLLERID Get_ControllerID();
inline _int Get_CountScene();
inline	HRESULT	Set_Scene(CScene* _pScene);
inline void Set_ControllerID(CONTROLLERID _eController);
inline	_int Update_Scene(const _float& _fTimeDelta);
inline	void LateUpdate_Scene();
inline	void Render_Scene(LPDIRECT3DDEVICE9 _pGraphicDev);
inline void Set_PlayerSkillTimer(const _float& _fTimer);

// ComponentManager
inline HRESULT Ready_Proto(const _tchar* _pComponentTag, CComponent* _pComponent);
inline CComponent* Clone_Proto(const _tchar* _pComponentTag);

// Renderer
inline void Ready_Renderer(LPDIRECT3DDEVICE9& _pGraphicDev);
inline void	Add_RenderGroup(RENDERID _eType, CGameObject* _pGameObject);
inline void	Render_GameObject(LPDIRECT3DDEVICE9& _pGraphicDev);
inline IDirect3DTexture9* Get_ChangedTexture();
inline IDirect3DSurface9* Get_PreSurface();
inline void Set_ChangeScreen(_bool _bChangeScreen);
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
inline list<CUI*>* Get_ListUI(UITYPE _eUIType);
inline HRESULT Add_UI(CUI* _pUI);
inline _int Update_UI(const _float& _fTimeDelta);
inline void LateUpdate_UI();
inline void Render_UI(LPDIRECT3DDEVICE9& _pGraphicDev);
inline CUI* Activate_UI(UITYPE _eUIType);
inline void Deactivate_UI(UITYPE _eUIType);

// CollisionManager
inline _bool Get_ColliderRender();
inline void Toggle_Collider();
inline void Update_Collision();
inline void Render_Collider();
inline void Add_Collider(CCollider* _pCollider);
inline void CheckGroup(const _tchar* _pLeft, const _tchar* _pRight);
inline void Clear_Collider();
inline void Reset();
//Beomseung
inline _bool RayCast(_vec3 vRayStart, _vec3 vRayDir);
inline _bool RayCast2(_vec3 vRayStart, _vec3 vRayDir);
inline _float FloorRayCast(_vec3 vRayStart);
inline _bool FireRayCast(_vec3 _vRayStart, _vec3 _vRayDir, _vec3& _vOut, const _float& _fDamage);
//유빈
inline CGameObject* FloorRayCast2(_vec3 vRayStart);//산성, 용암 바닥 타일 정보 가져오기 위해 만듬

//Jonghan Change
inline HRESULT Add_Bullet(CBullet* _pBullet);
inline HRESULT Add_Missile(CBullet* _pMissile);
inline HRESULT Add_Laser(CBullet* _pLaser);
inline HRESULT Add_MiniGun(CBullet* _pMiniGun);
inline HRESULT Add_HumanoidHead(CBullet* _pHead);
inline HRESULT Add_Boss_Humanoid_Laser(CBullet* _pBossLaser);
inline HRESULT Fire_Bullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage, CBulletManager::BULLETTYPE _eBulletType, const _bool& _bIsBoss = false, const _vec3& vCurvePos = { 0.f,0.f,0.f });
inline _float Get_Bullet_Linear(CBulletManager::BULLETTYPE _eBulletType);
inline _int Update_Bullet(const _float& _fTimeDelta);
inline void LateUpdate_Bullet();

inline void Set_Trigger(_int _iTrigger, CGameObject* _pGameObject);
inline void Calculate_Trigger();
inline void Collision_With_Trigger(const _int& _iTriggerNumber);

inline void Set_DrinkObject(CSoda* _pGameObject);
inline void Spawn_DrinkObject(_vec3 _vStartPos);

inline void	Release_Utility();

#include "Export_Utility.inl"

END