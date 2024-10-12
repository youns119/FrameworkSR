inline HRESULT Create_Management(LPDIRECT3DDEVICE9 _pGraphicDev, CManagement** _ppManagement)
{
	CManagement* pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*_ppManagement = pManagement;

	return S_OK;
}

inline CScene* Get_CurrScene()
{
	return CManagement::GetInstance()->Get_CurrScene();
}

inline CONTROLLERID Get_ControllerID()
{
	return CManagement::GetInstance()->Get_ControllerID();
}

inline _int Get_CountScene()
{
	return CManagement::GetInstance()->Get_CountScene();
}

inline CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag)
{
	return CManagement::GetInstance()->Get_Component(_eID, _pLayerTag, _pObjTag, _pComponentTag);
}

inline HRESULT Set_Scene(CScene* _pScene)
{
	return CManagement::GetInstance()->Set_Scene(_pScene);
}

inline void Set_ControllerID(CONTROLLERID _eController)
{
	CManagement::GetInstance()->Set_ControllerID(_eController);
}

inline _int Update_Scene(const _float& _fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(_fTimeDelta);
}

inline void LateUpdate_Scene()
{
	CManagement::GetInstance()->LateUpdate_Scene();
}

inline void Render_Scene(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(_pGraphicDev);
}

inline void Set_PlayerSkillTimer(const _float& _fTimer)
{
	CManagement::GetInstance()->Set_PlayerSkillTimer(_fTimer);
}

// ComponentManager
inline HRESULT Ready_Proto(const _tchar* _pComponentTag, CComponent* _pComponent)
{
	return CComponentManager::GetInstance()->Ready_Proto(_pComponentTag, _pComponent);
}

inline CComponent* Clone_Proto(const _tchar* _pComponentTag)
{
	return CComponentManager::GetInstance()->Clone_Proto(_pComponentTag);
}

// Renderer
inline void Ready_Renderer(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	CRenderer::GetInstance()->Ready_Renderer(_pGraphicDev);
}

inline void	Add_RenderGroup(RENDERID _eType, CGameObject* _pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(_eType, _pGameObject);
}

inline void	Render_GameObject(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(_pGraphicDev);
}

inline IDirect3DTexture9* Get_ChangedTexture()
{
	return CRenderer::GetInstance()->Get_ChangedTexture();
}

inline IDirect3DSurface9* Get_PreSurface()
{
	return CRenderer::GetInstance()->Get_PreSurface();
}

inline void Set_ChangeScreen(_bool _bChangeScreen)
{
	CRenderer::GetInstance()->Set_ChangeScreen(_bChangeScreen);
}

inline void	Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

inline HRESULT Ready_Light
(
	LPDIRECT3DDEVICE9 _pGraphicDev,
	const D3DLIGHT9* _pLightInfo,
	const _uint& _iIndex
)
{
	return CLightManager::GetInstance()->Ready_Light(_pGraphicDev, _pLightInfo, _iIndex);
}

// ¿¬¿í
// UIManager
inline list<CUI*>* Get_ListUI(UITYPE _eUIType)
{
	return CUIManager::GetInstance()->Get_ListUI(_eUIType);
}

inline HRESULT Add_UI(CUI* _pUI)
{
	return CUIManager::GetInstance()->Add_UI(_pUI);
}

inline _int Update_UI(const _float& _fTimeDelta)
{
	return CUIManager::GetInstance()->Update_UI(_fTimeDelta);
}

inline void LateUpdate_UI()
{
	CUIManager::GetInstance()->LateUpdate_UI();
}

inline void Render_UI(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	CUIManager::GetInstance()->Render_UI(_pGraphicDev);
}

inline CUI* Activate_UI(UITYPE _eUIType)
{
	return CUIManager::GetInstance()->Activate_UI(_eUIType);
}

inline void Deactivate_UI(UITYPE _eUIType)
{
	CUIManager::GetInstance()->Deactivate_UI(_eUIType);
}

// CollisionManager
inline _bool Get_ColliderRender()
{
	return CCollisionManager::GetInstance()->Get_ColliderRender();
}

inline void Toggle_Collider()
{
	CCollisionManager::GetInstance()->Toggle_Collider();
}

inline void Update_Collision()
{
	CCollisionManager::GetInstance()->Update_Collision();
}

inline void Render_Collider()
{
	CCollisionManager::GetInstance()->Render_Collider();
}

inline void Add_Collider(CCollider* _pCollider)
{
	CCollisionManager::GetInstance()->Add_Collider(_pCollider);
}

inline void CheckGroup(const _tchar* _pLeft, const _tchar* _pRight)
{
	CCollisionManager::GetInstance()->CheckGroup(_pLeft, _pRight);
}

inline void Clear_Collider()
{
	CCollisionManager::GetInstance()->Clear_Collider();
}

inline void Reset()
{
	CCollisionManager::GetInstance()->Reset();
}

//Beomseung
inline _bool RayCast(_vec3 vRayStart, _vec3 vRayDir)
{
	return CCollisionManager::GetInstance()->RayCast(vRayStart, vRayDir);
}
inline _bool RayCast2(_vec3 vRayStart, _vec3 vRayDir)
{
	return CCollisionManager::GetInstance()->RayCast2(vRayStart, vRayDir);
}

inline _float FloorRayCast(_vec3 vRayStart)
{
	return CCollisionManager::GetInstance()->FloorRayCast(vRayStart);
}

inline _bool FireRayCast(_vec3 _vRayStart, _vec3 _vRayDir, _vec3& _vOut, const _float& _fDamage)
{
	return CCollisionManager::GetInstance()->FireRayCast(_vRayStart, _vRayDir, _vOut, _fDamage);
}

inline CGameObject* FloorRayCast2(_vec3 vRayStart)
{
	return CCollisionManager::GetInstance()->FloorRayCast2(vRayStart);
}

//Jonghan Change
inline HRESULT Add_Bullet(CBullet* _pBullet)
{
	return CBulletManager::GetInstance()->Add_Bullet(_pBullet);
}

inline HRESULT Add_Missile(CBullet* _pMissile)
{
	return CBulletManager::GetInstance()->Add_Missile(_pMissile);
}

inline HRESULT Add_Laser(CBullet* _pLaser)
{
	return CBulletManager::GetInstance()->Add_Laser(_pLaser);
}

inline HRESULT Add_MiniGun(CBullet* _pMiniGun)
{
	return CBulletManager::GetInstance()->Add_MiniGun(_pMiniGun);
}

inline HRESULT Add_HumanoidHead(CBullet* _pHead)
{
	return CBulletManager::GetInstance()->Add_HumanoidHead(_pHead);
}

inline HRESULT Add_Boss_Humanoid_Laser(CBullet* _pBossLaser)
{
	return CBulletManager::GetInstance()->Add_Boss_Humanoid_Laser(_pBossLaser);
}

inline HRESULT Fire_Bullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage, CBulletManager::BULLETTYPE _eBulletType, const _bool& _bIsBoss, const _vec3& vCurvePos)
{
	return CBulletManager::GetInstance()->Fire_Bullet(_pGraphicDev, _vStartPos, _vDir, _fAttackDamage, _eBulletType, _bIsBoss, vCurvePos);
}

inline _float Get_Bullet_Linear(CBulletManager::BULLETTYPE _eBulletType)
{
	return CBulletManager::GetInstance()->Get_Bullet_Linear(_eBulletType);
}

inline _int Update_Bullet(const _float& _fTimeDelta)
{
	return CBulletManager::GetInstance()->Update_Bullet(_fTimeDelta);
}

inline void LateUpdate_Bullet()
{
	CBulletManager::GetInstance()->LateUpdate_Bullet();
}

inline void Set_Trigger(_int _iTrigger, CGameObject* _pGameObject)
{
	CTrigger::GetInstance()->Set_Trigger(_iTrigger, _pGameObject);
}

inline void Calculate_Trigger()
{
	CTrigger::GetInstance()->Calculate_Trigger();
}

inline void Collision_With_Trigger(const _int& _iTriggerNumber)
{
	CTrigger::GetInstance()->Collision_With_Trigger(_iTriggerNumber);
}

inline void Set_DrinkObject(CSoda* _pGameObject)
{
	CDrinkSpawner::GetInstance()->Set_DrinkObject(_pGameObject);
}

inline void Spawn_DrinkObject(_vec3 _vStartPos)
{
	CDrinkSpawner::GetInstance()->Spawn_DrinkObject(_vStartPos);
}

void Release_Utility()
{
	CLightManager::DestroyInstance();
	CRenderer::DestroyInstance();
	CComponentManager::DestroyInstance();
	CManagement::DestroyInstance();
	CUIManager::DestroyInstance();
	CCollisionManager::DestroyInstance();
	CBulletManager::DestroyInstance();
	CTrigger::DestroyInstance();
	CDrinkSpawner::DestroyInstance();
}