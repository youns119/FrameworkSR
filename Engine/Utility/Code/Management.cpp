#include "..\..\Header\Management.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement() 
	: m_pScene(nullptr)
{
}

CManagement::~CManagement()
{
	Free();
}

CComponent* CManagement::Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag)
{
	NULL_CHECK_RETURN(m_pScene, nullptr);

	return m_pScene->Get_Component(_eID, _pLayerTag, _pObjTag, _pComponentTag);
}

HRESULT CManagement::Set_Scene(CScene* _pScene)
{
	Safe_Release(m_pScene);

	m_pScene = _pScene;

	return S_OK;
}

_int CManagement::Update_Scene(const _float& _fTimeDelta)
{
	NULL_CHECK_RETURN(m_pScene, -1);

	return m_pScene->Update_Scene(_fTimeDelta);
}

void CManagement::LateUpdate_Scene()
{
	NULL_CHECK(m_pScene);
	m_pScene->LateUpdate_Scene();
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	NULL_CHECK(m_pScene);
	m_pScene->Render_Scene();
}

void CManagement::Free()
{
	Safe_Release(m_pScene);
}