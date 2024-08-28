inline HRESULT Create_Management(LPDIRECT3DDEVICE9 _pGraphicDev, CManagement** _ppManagement)
{
	CManagement* pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*_ppManagement = pManagement;
	
	return S_OK;
}

inline CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag)
{
	return CManagement::GetInstance()->Get_Component(_eID, _pLayerTag, _pObjTag, _pComponentTag);
}

inline HRESULT Set_Scene(CScene* _pScene)
{
	return CManagement::GetInstance()->Set_Scene(_pScene);
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
inline void	Add_RenderGroup(RENDERID _eType, CGameObject* _pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(_eType, _pGameObject);
}

inline void	Render_GameObject(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(_pGraphicDev);
}

inline void	Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

void Release_Utility()
{
	CRenderer::DestroyInstance();
	CComponentManager::DestroyInstance();
	CManagement::DestroyInstance();
}