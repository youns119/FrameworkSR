inline HRESULT Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement)
{
	CManagement* pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagement = pManagement;
	
	return S_OK;
}

inline CComponent* Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	return CManagement::GetInstance()->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

inline HRESULT Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}

inline _int Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}

inline void LateUpdate_Scene()
{
	CManagement::GetInstance()->LateUpdate_Scene();
}

inline void Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

void Release_Utility()
{
	CManagement::DestroyInstance();
}