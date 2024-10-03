#include "..\..\Header\Scene.h"

CScene::CScene(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
{
	m_mapLayer.clear();

	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

const _tchar* CScene::Get_Layer(CGameObject* _pGameObject)
{
	for (auto iter = m_mapLayer.begin(); iter != m_mapLayer.end(); ++iter)
	{
		if (iter->second->Get_GameObject(_pGameObject))
			return iter->first;
		else continue;
	}

	return nullptr;
}

multimap<const _tchar*, CGameObject*>* CScene::Get_LayerObjects(const _tchar* _pLayerTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_LayerObjects();
}

CGameObject* CScene::Get_GameObject(const _tchar* _pLayerTag, const _tchar* _pObjTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(_pObjTag);
}

CComponent* CScene::Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(_eID, _pObjTag, _pComponentTag);
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float& _fTimeDelta)
{
	_int iResult(0);

	for (auto& pLayer : m_mapLayer)
	{
		iResult = pLayer.second->Update_Layer(_fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CScene::LateUpdate_Scene()
{
	for (auto& pLayer : m_mapLayer)
		pLayer.second->LateUpdate_Layer();
}

void CScene::Free()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}